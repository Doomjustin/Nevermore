#ifndef NEVERMORE_STRING_UTILS_H
#define NEVERMORE_STRING_UTILS_H

#include <string>
#include <string_view>

namespace sf {

struct Charset {
    static constexpr char Space = ' ';
    static constexpr char Dot = '.';
    static constexpr char Colon = ':';
    static constexpr char Comma = ',';
    static constexpr char Slash = '/';
};

template<typename Char, typename CharT>
std::basic_string<Char, CharT> trim_left(const std::basic_string<Char, CharT>& str, Char space = Charset::Space)
{
    auto pos = str.find_first_not_of(space);
    return str.substr(pos);
}

template<typename Char, typename CharT>
std::basic_string<Char, CharT> trim_right(const std::basic_string<Char, CharT>& str, Char space = Charset::Space)
{
    auto pos = str.find_last_not_of(space);
    return str.substr(0, pos + 1);
}

template<typename Char, typename CharT>
std::basic_string<Char, CharT> trim(const std::basic_string<Char, CharT>& str, Char space = Charset::Space)
{
    auto start_pos = str.find_first_not_of(space);
    auto end_pos = str.find_last_not_of(space);
    return str.substr(start_pos, end_pos - start_pos + 1);
}

template<typename Char, typename CharT>
std::basic_string_view<Char, CharT> trim_left(std::basic_string_view<Char, CharT> str, Char space = Charset::Space)
{
    auto pos = str.find_first_not_of(space);
    return str.substr(pos);
}

template<typename Char, typename CharT>
std::basic_string_view<Char, CharT> trim_right(std::basic_string_view<Char, CharT> str, Char space = Charset::Space)
{
    auto pos = str.find_last_not_of(space);
    return str.substr(0, pos + 1);
}

template<typename Char, typename CharT>
std::basic_string_view<Char, CharT> trim(std::basic_string_view<Char, CharT> str, Char space = Charset::Space)
{
    auto start_pos = str.find_first_not_of(space);
    auto end_pos = str.find_last_not_of(space);
    return str.substr(start_pos, end_pos - start_pos + 1);
}

} // namespace sf

#endif // NEVERMORE_STRING_UTILS_H