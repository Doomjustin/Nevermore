#ifndef NEVERMORE_LEVELDB_DATABASE_H
#define NEVERMORE_LEVELDB_DATABASE_H

#include <deque>
#include <mutex>
#include <string>
#include <string_view>
#include <condition_variable>

#include "WriteBatch.h"

namespace sf {

class Database {
public:
    using SizeType = typename WriteBatch::SizeType;
    
public:
    void put(std::string_view key, std::string_view value);

    void erase(std::string_view key);

    std::string_view get(std::string_view key) const;

    void write(WriteBatch batch);

private:
    struct Writer {
        WriteBatch batch;
        bool sync;
        bool done;
        std::condition_variable cv;
    };

private:
    std::string name_;
    std::mutex m_;
    std::condition_variable ready_to_write_;
    std::deque<Writer*> writers_;

    constexpr bool is_first_writer(const Writer* writer) const noexcept
    {
        return !writer->done && writer == writers_.front();
    }

    std::pair<WriteBatch, Writer*> build_batch_group(Writer& writer);
};

} // namespace sf

#endif // NEVERMORE_LEVELDB_DATABASE_H