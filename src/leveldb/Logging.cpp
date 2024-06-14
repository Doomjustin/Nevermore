#include "nevermore/Logging.h"
#include <cstdint>
#include <cstdio>
#include <limits>

namespace sf {

void append_number_to(std::string& str, std::uint64_t num)
{
    char buf[30];
    std::snprintf(buf, sizeof(buf), "%llu", static_cast<unsigned long long>(num));
    str.append(buf);
}

void append_escaped_string_to(std::string& str, std::string_view value)
{
    for (auto i = 0; i < value.size(); ++i) {
        auto c = value[i];
        if (c >= ' ' && c <= '~') {
            str.push_back(c);
        }
        else {
            char buf[10];
            std::snprintf(buf, sizeof(buf), "\\x%02x",
                          static_cast<unsigned int>(c) & 0xff);
            str.append(buf);
        }
    }   
}

std::string number_to_string(std::uint64_t num)
{
    std::string r;
    append_number_to(r, num);

    return r;
}

std::string escape_string(std::string_view value)
{
    std::string r;
    append_escaped_string_to(r, value);

    return r;
}

bool consume_decimal_number(std::string_view& in, std::uint64_t& val)
{
    constexpr auto MaxUint64 = std::numeric_limits<std::uint64_t>::max();
    constexpr char LastDigitOfMaxUint64 = '0' + static_cast<char>(MaxUint64 % 10);

    std::uint64_t value = 0;
    const std::uint8_t* start = reinterpret_cast<const std::uint8_t*>(in.data());
    const std::uint8_t* end = start + in.size();
    const std::uint8_t* current = start;
    for ( ;current != end; ++current) {
        auto ch = *current;
        if (ch < '0' || ch > '9') break;

        if (value > MaxUint64 / 10 ||
            (value = MaxUint64 / 10 && ch > LastDigitOfMaxUint64))
            return false;

        value = value * 10 + (ch - '0');
    }

    val = value;
    auto digits_consumed = current - start;
    in.remove_prefix(digits_consumed);
    return digits_consumed != 0;
}

} // namespace sf