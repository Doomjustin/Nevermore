#ifndef NEVERMORE_DATABASE_DATABASE_H
#define NEVERMORE_DATABASE_DATABASE_H

#include <string_view>
#include <string>

#include "Status.h"
#include "Option.h"
#include "WriteBatch.h"

namespace sf {

class Snapshot {
public:
    virtual ~Snapshot() = default;
};

struct Range {
    Range() = default;
    Range(std::string_view s, std::string l)
      : start{ s }, limit{ l }
    {}

    std::string_view start;
    std::string limit;
};

class LevelDB {
public:
    static Status open(const Options& options, const std::string& name, LevelDB** dbptr);

    LevelDB() = default;

    LevelDB(const LevelDB&) = delete;
    LevelDB& operator=(const LevelDB&) = delete;

    ~LevelDB() = default;

    Status put(const WriteOption& options, std::string_view key, std::string_view value);
    Status remove(const WriteOption& options, std::string_view key);
    Status write(const WriteOption& options, const WriteBatch& batch);
    Status get(const ReadOption& options, std::string_view key, std::string_view& value);

private:
    struct Writer;

    struct ManualCompaction {
        int level;
        bool done;
    };
};

} // namespace sf

#endif // NEVERMORE_DATABASE_DATABASE_H