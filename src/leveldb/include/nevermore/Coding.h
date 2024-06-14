#ifndef NEVERMORE_DATABASE_CODING_H
#define NEVERMORE_DATABASE_CODING_H

#include <cstdint>
#include <string>

namespace sf {

void put_fixed32(std::string& dst, std::uint32_t value);
void put_fixed64(std::string& dst, std::uint64_t value);

void put_varint32(std::string& dst, std::uint32_t value);
void put_varint64(std::string& dst, std::uint64_t value);

void put_length_prefixed_slice(std::string& dst, std::string_view value);
bool get_length_prefixed_slice(std::string_view& input, std::string_view& result);

void encode_fixed32(char* dst, std::uint32_t value);
void encode_fixed64(char* dst, std::uint64_t value);

char* encode_varint32(char* dst, std::uint32_t value);
char* encode_varint64(char* dst, std::uint64_t value);

bool get_varint32(std::string_view& input, std::uint32_t& value);
bool get_varint64(std::string_view& input, std::uint64_t& value);

const char* get_varint32_ptr_fallback(const char* p, const char* limit, std::uint32_t& value);
const char* get_varint32_ptr(const char* p, const char* limit, std::uint32_t& value);
const char* get_varint64_ptr(const char* p, const char* limit, std::uint64_t& value);

std::uint32_t decode_fixed32(const char* ptr);
std::uint64_t decode_fixed64(const char* ptr);

int varint_length(std::uint64_t v);

} // namespace sf

#endif // NEVERMORE_DATABASE_CODING_H