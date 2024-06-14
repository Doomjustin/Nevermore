#ifndef NEVERMORE_DATABASE_INTERNAL_KEY_H
#define NEVERMORE_DATABASE_INTERNAL_KEY_H

#include <string>
#include <string_view>

#include "Format.h"

namespace sf {

struct ParsedInternalKey {
    std::string_view user_key;
    SequenceNumber sequence;
    ValueType type;

    std::string to_string() const;
};

// Modules in this directory should keep internal keys wrapped inside
// the following class instead of plain strings so that we do not
// incorrectly use string comparisons instead of an InternalKeyComparator.
class InternalKey {
public:
public:
    InternalKey() = default;

    InternalKey(std::string_view user_key, SequenceNumber seq, ValueType type);

    bool decode_from(std::string_view s) noexcept;

    std::string_view encode() const;
    
    std::string_view user_key() const;

    void set_from(const ParsedInternalKey& p);

    void clear();

    std::string to_string() const;

private:
    std::string rep_;
};

bool parse_internal_key(std::string_view internal_key, ParsedInternalKey& result);

} // namespace sf

#endif // NEVERMORE_DATABASE_INTERNAL_KEY_H