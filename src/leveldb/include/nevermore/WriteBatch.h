#ifndef NEVERMORE_DATABASE_WRITE_BATCH_H
#define NEVERMORE_DATABASE_WRITE_BATCH_H

#include <string>
#include <string_view>

#include "Status.h"

namespace sf {

class WriteBatch {
friend class WriteBatchInternal;

public:
    struct Handler {
        virtual ~Handler() = default;
        virtual void put(const std::string_view key, const std::string_view value) = 0;
        virtual void remove(const std::string_view key) = 0;
    };

    WriteBatch();

    WriteBatch(const WriteBatch&) = default;
    WriteBatch& operator=(const WriteBatch&) = default;

    ~WriteBatch() = default;

    /**
     * @brief Store the mapping "key->value" in the database.
     * 
     * @param key 
     * @param value 
     */
    void put(const std::string_view key, const std::string_view value);

    /**
     * @brief If the database contains a mapping for "key", erase it.  Else do nothing.
     * 
     * @param key 
     */
    void remove(const std::string_view key);

    /**
     * @brief Clear all updates buffered in this batch.
     * 
     */
    void clear();

    /**
     * @brief The size of the database changes caused by this batch.
     * @details This number is tied to implementation details, and may change across releases.
     * It is intended for LevelDB usage metrics.
     * 
     * @return std::size_t 
     */
    std::size_t approximate_size();

    /**
     * @brief Copies the operations in "source" to this batch.
     * @details This runs in O(source size) time. However, the constant factor is better
     * than calling Iterate() over the source batch with a Handler that replicates the operations into this batch.
     * 
     * @param other 
     */
    void append(const WriteBatch& other);

    Status iterator(Handler* handler) const;

private:

    std::string rep_;
};

} // namespace sf

#endif // NEVERMORE_DATABASE_WRITE_BATCH_H