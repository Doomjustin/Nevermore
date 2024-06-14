#include "nevermore/Coding.h"

#include <cstdint>
#include <string_view>

namespace sf {

static constexpr std::uint8_t byte_size = 8;

static constexpr std::uint8_t varint_byte_size = 7;
static constexpr std::uint8_t varint_max_byte_value = 1 << 7;

void put_fixed32(std::string& dst, std::uint32_t value)
{
    constexpr auto value_size = sizeof(value);
    char buf[value_size];
    encode_fixed32(buf, value);
    dst.append(buf, value_size);
}

void put_fixed64(std::string& dst, std::uint64_t value)
{
    constexpr auto value_size = sizeof(value);
    char buf[value_size];
    encode_fixed64(buf, value);
    dst.append(buf, value_size);
}

void put_varint32(std::string& dst, std::uint32_t value)
{
    char buf[5];
    auto* ptr = encode_varint32(buf, value);
    dst.append(buf, ptr - buf);
}

void put_varint64(std::string& dst, std::uint64_t value)
{
    char buf[10];
    char* ptr = encode_varint64(buf, value);
    dst.append(buf, ptr - buf);
}

void put_length_prefixed_slice(std::string& dst, std::string_view value)
{
    put_varint32(dst, value.size());
    dst.append(value);
}

bool get_length_prefixed_slice(std::string_view& input, std::string_view& result)
{
    std::uint32_t len;
    if (get_varint32(input, len) && input.size() >= len) {
        result = std::string_view{ input.data(), len };
        input.remove_prefix(len);
        return true;
    }

    return false;
}

void encode_fixed32(char* dst, std::uint32_t value)
{
    auto* const buffer = reinterpret_cast<std::uint8_t*>(dst);
    buffer[0] = static_cast<std::uint8_t>(value);
    buffer[1] = static_cast<std::uint8_t>(value >> byte_size);
    buffer[2] = static_cast<std::uint8_t>(value >> (2 * byte_size));
    buffer[3] = static_cast<std::uint8_t>(value >> (3 * byte_size));
}   

void encode_fixed64(char* dst, std::uint64_t value)
{
    auto* const buffer = reinterpret_cast<std::uint8_t*>(dst);
    buffer[0] = static_cast<std::uint8_t>(value);
    buffer[1] = static_cast<std::uint8_t>(value >> byte_size);
    buffer[2] = static_cast<std::uint8_t>(value >> (2 * byte_size));
    buffer[3] = static_cast<std::uint8_t>(value >> (3 * byte_size));
    buffer[4] = static_cast<std::uint8_t>(value >> (4 * byte_size));
    buffer[5] = static_cast<std::uint8_t>(value >> (5 * byte_size));
    buffer[6] = static_cast<std::uint8_t>(value >> (6 * byte_size));
    buffer[7] = static_cast<std::uint8_t>(value >> (7 * byte_size));
}

char* encode_varint32(char* dst, std::uint32_t value)
{
    auto* ptr = reinterpret_cast<std::uint8_t*>(dst);
    if (value < (1 << varint_byte_size)) {
        *(ptr++) = value;
    } 
    else if (value < (1 << (2 * varint_byte_size))) {
        *(ptr++) = value | varint_max_byte_value;
        *(ptr++) = value >> varint_byte_size;
    }
    else if (value < (1 << (3 * varint_byte_size))) {
        *(ptr++) = value | varint_max_byte_value;
        *(ptr++) = (value >> varint_byte_size) | varint_max_byte_value;
        *(ptr++) = value >> (2 * varint_byte_size);
    } 
    else if (value < (1 << (4 * varint_byte_size))) {
        *(ptr++) = value | varint_max_byte_value;
        *(ptr++) = (value >> varint_byte_size) | varint_max_byte_value;
        *(ptr++) = value >> (2 * varint_byte_size) | varint_max_byte_value;
        *(ptr++) = value >> (3 * varint_byte_size);
    }
    else {
        *(ptr++) = value | varint_max_byte_value;
        *(ptr++) = (value >> varint_byte_size) | varint_max_byte_value;
        *(ptr++) = value >> (2 * varint_byte_size) | varint_max_byte_value;
        *(ptr++) = value >> (3 * varint_byte_size) | varint_max_byte_value;
        *(ptr++) = value >> (4 * varint_byte_size);
    }

    return reinterpret_cast<char*>(ptr);
}

char* encode_varint64(char* dst, std::uint64_t value)
{
    auto* ptr = reinterpret_cast<std::uint8_t*>(dst);

    while (value >= varint_max_byte_value) {
        *(ptr++) = value | varint_max_byte_value;
        value >>= varint_byte_size;
    }
    *(ptr++) = static_cast<std::uint8_t>(value);
    return reinterpret_cast<char*>(ptr);
}

bool get_varint32(std::string_view& input, std::uint32_t& value)
{
    const auto* p = input.data();
    const char* limit = p + input.size();
    const char* q = get_varint32_ptr(p, limit, value);
    if (q == nullptr)
        return false;

    using SizeType = typename std::string_view::size_type;
    input = std::string_view{ q, static_cast<SizeType>(limit - q) };
    return true;
}

bool get_varint64(std::string_view& input, std::uint64_t& value)
{
    const char* p = input.data();
    const char* limit = p + input.size();
    const char* q = get_varint64_ptr(p, limit, value);
    if (q == nullptr)
        return false;

    using SizeType = typename std::string_view::size_type;
    input = std::string_view{ q, static_cast<SizeType>(limit - q) };
    return true;
}

const char* get_varint32_ptr_fallback(const char* p, const char* limit, std::uint32_t& value)
{
    constexpr auto value_bytes = sizeof(value);
    std::uint32_t result = 0;
    for (auto shift = 0; shift <= (value_bytes * varint_byte_size) && p < limit; shift += varint_byte_size) {
        std::uint32_t byte = *reinterpret_cast<const std::uint8_t*>(p);
        ++p;
        if (byte & varint_max_byte_value) {
            result |= ((byte & 127) << shift);
        }
        else {
            result |= (byte << shift);
            value = result;
            return reinterpret_cast<const char*>(p); 
        }
    }

    return nullptr;
}

const char* get_varint32_ptr(const char* p, const char* limit, std::uint32_t& value)
{
    constexpr std::uint8_t is_not_last_value_byte = 0b10000000;
    if (p < limit) {
        std::uint32_t result = *reinterpret_cast<const std::uint8_t*>(p);
        if ((result & is_not_last_value_byte) == 0) {
            value = result;
            return p + 1;
        }
    }

    return get_varint32_ptr_fallback(p, limit, value);
}

const char* get_varint64_ptr(const char* p, const char* limit, std::uint64_t& value)
{
    std::uint64_t result{};
    for (std::uint32_t shift = 0; shift <= 63 && p < limit; shift += 7) {
        std::uint64_t byte = *reinterpret_cast<const std::uint8_t*>(p);
        ++p;
        if (byte & varint_max_byte_value) {
            result |= ((byte & 127) << shift);
        }
        else {
            result |= (byte << shift);
            value = result;
            return reinterpret_cast<const char*>(p);
        }
    }

    return nullptr;
}

std::uint32_t decode_fixed32(const char* ptr)
{
    const auto* const buffer = reinterpret_cast<const std::uint8_t*>(ptr);

    return (static_cast<std::uint32_t>(buffer[0])) |
           (static_cast<std::uint32_t>(buffer[1]) << byte_size) |
           (static_cast<std::uint32_t>(buffer[2]) << (2 * byte_size)) |
           (static_cast<std::uint32_t>(buffer[3]) << (3 * byte_size));
}

std::uint64_t decode_fixed64(const char* ptr)
{
    const auto* const buffer = reinterpret_cast<const std::uint8_t*>(ptr);

    return (static_cast<std::uint64_t>(buffer[0])) |
           (static_cast<std::uint64_t>(buffer[1]) << byte_size) |
           (static_cast<std::uint64_t>(buffer[2]) << (2 * byte_size)) |
           (static_cast<std::uint64_t>(buffer[3]) << (3 * byte_size)) |
           (static_cast<std::uint64_t>(buffer[4]) << (4 * byte_size)) |
           (static_cast<std::uint64_t>(buffer[5]) << (5 * byte_size)) |
           (static_cast<std::uint64_t>(buffer[6]) << (6 * byte_size)) |
           (static_cast<std::uint64_t>(buffer[7]) << (7 * byte_size));
}

int varint_length(std::uint64_t v)
{
    int len = 1;
    while (v >= varint_max_byte_value) {
        v >>= varint_byte_size;
        ++len;
    }

    return len;
}

} // namespace sf
