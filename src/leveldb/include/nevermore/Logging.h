#ifndef NEVERMORE_DATABASE_LOGGING_H
#define NEVERMORE_DATABASE_LOGGING_H

#include <cstdint>
#include <string>
#include <string_view>

namespace sf {

// Append a human-readable printout of "num" to str
void append_number_to(std::string& str, std::uint64_t num);

// Append a human-readable printout of "value" to *str.
// Escapes any non-printable characters found in "value".
void append_escaped_string_to(std::string& str, std::string_view value);

// Return a human-readable printout of "num"
std::string number_to_string(std::uint64_t num);

// Return a human-readable version of "value".
// Escapes any non-printable characters found in "value".
std::string escape_string(std::string_view value);

// Parse a human-readable number from "*in" into *value.  On success,
// advances "*in" past the consumed number and sets "*val" to the
// numeric value.  Otherwise, returns false and leaves *in in an
// unspecified state.
bool consume_decimal_number(std::string_view& in, std::uint64_t& val);

} // namespace sf

#endif // NEVERMORE_DATABASE_LOGGING_H