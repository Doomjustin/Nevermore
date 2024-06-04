#ifndef NEVERMORE_BITS_H
#define NEVERMORE_BITS_H

#include <type_traits>

namespace sf::bits {

template<typename T>
    requires std::is_integral_v<T>
consteval T left_shift(T n, T base = 1)
{
    return base << n;
}

template<typename T>
    requires std::is_integral_v<T>
consteval T right_shift(T n, T base = 1)
{
    return base >> n;
}

} // namespace sf::bits

#endif // NEVERMORE_BITS_H