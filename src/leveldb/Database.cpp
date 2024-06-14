#include "nevermore/Database.h"
#include "nevermore/Status.h"
#include "nevermore/WriteBatch.h"

namespace sf {

Status LevelDB::put(const WriteOption& options, std::string_view key, std::string_view value)
{
    WriteBatch batch{};
    batch.put(key, value);
    return write(options, batch);
}

Status LevelDB::remove(const WriteOption& options, std::string_view key)
{
    WriteBatch batch{};
    batch.remove(key);
    return write(options, batch);
}

Status LevelDB::write(const WriteOption& options, const WriteBatch& batch)
{
    return Status::ok();
}

Status LevelDB::get(const ReadOption& options, std::string_view key, std::string_view& value)
{
    return Status::ok();
}

} // namespace sf