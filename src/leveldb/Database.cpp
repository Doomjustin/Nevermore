#include "nevermore/Database.h"
#include "nevermore/WriteBatch.h"

#include <cassert>
#include <mutex>
#include <utility>

namespace sf {


void Database::put(std::string_view key, std::string_view value)
{
    WriteBatch batch;
    batch.add(key, value);
    write(std::move(batch));
}

void Database::erase(std::string_view key)
{
    WriteBatch batch;
    batch.erase(key);
    write(std::move(batch));
}

std::string_view Database::get(std::string_view key) const
{
    return "unimplement yet";
}

void Database::write(WriteBatch batch)
{
    Writer writer{
        std::move(batch),
        true,
        false
    };
    
    std::unique_lock<std::mutex> locker{ m_ };
    writers_.emplace_back(&writer);
    ready_to_write_.wait(locker, [&writer, this] () { return is_first_writer(&writer); });

    if (writer.done) return;

    // TODO: MakeRoomForWrite
    // get last sequence number

    auto [actual_batch, last_writer] = build_batch_group(writer);
    // TODO: set sequence number

    locker.unlock();
    // TODO: log输出write_batch
    if (writer.sync) {
        // TODO: log file sync
    }
    
    locker.lock();
    
    // 移除所有已经完成的writer
    while (true) {
        auto* completed_writer = writers_.front();
        writers_.pop_front();
        if (completed_writer != &writer) {
            completed_writer->done = true;
            ready_to_write_.notify_one();
        }

        if (completed_writer == last_writer) break;
    }

    if (!writers_.empty())
        ready_to_write_.notify_one();
}

/**
 * @brief 
 * 
 * @param last_writer 
 * @return WriteBatch* 
 * @pre writers_ must be non-empty, and is locked by unique_lock
 */
std::pair<WriteBatch, Database::Writer*> Database::build_batch_group(Writer& writer)
{
    assert(!writers_.empty());
    auto* first_writer = writers_.front();
    auto& result = first_writer->batch;

    // Allow the group to grow up to a maximum size, but if the
    // original write is small, limit the growth so we do not slow
    // down the small write too much.
    auto max_size = 1 << 20;
    auto batch_size = result.decode_size();
    if (batch_size <= (128 << 10))
        max_size = batch_size + (128 << 10);

    auto last_writer = first_writer;
    auto iter = std::next(writers_.begin(), 1);
    for (; iter != writers_.end(); ++iter) {
        auto cur_writer = *iter;
        if (cur_writer->sync && !first_writer->sync)
            break;

        batch_size += cur_writer->batch.decode_size();
        if (batch_size > max_size)
            break;

        result += cur_writer->batch;

        last_writer = cur_writer;
    }

    return std::make_pair(result, last_writer);
}

} // namespace sf