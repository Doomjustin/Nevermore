#ifndef NEVERMORE_DATABASE_FORMAT_H
#define NEVERMORE_DATABASE_FORMAT_H

#include <cstdint>

namespace sf {

enum class ValueType: std::uint8_t {
    Deletion = 0x0,
    Value = 0x1
};

static constexpr ValueType ValueTypeForSeek = ValueType::Value;

using SequenceNumber = std::uint64_t;

static constexpr SequenceNumber MaxSequenceNumber = ((0x1ull << 56) - 1);

} // namespace sf

#endif // NEVERMORE_DATABASE_FORMAT_H