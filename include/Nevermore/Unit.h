#ifndef NEVERMORE_UNIT_H
#define NEVERMORE_UNIT_H

#include <type_traits>


namespace SF {

template<int M, int K, int S>
struct Unit {
    enum { m = M, kg = K, s = S };
};

using M = Unit<1, 0, 0>;        // 米
using Kg = Unit<0, 1, 0>;       // 千克
using S = Unit<0, 0, 1>;        // 秒
using MpS = Unit<1, 0, -1>;     // 米/秒
using MpS2 = Unit<1, 0, -2>;    // 米/秒^2

// Unit<0, 0, 0>表示一个无量纲实体，即一个没有单位的值

template<typename U1, typename U2>
struct UPlus {
    using type = Unit<U1::m + U2::m, U1::kg + U2::kg, U1::s + U2::s>;
};

template<typename U1, typename U2>
struct UMinus {
    using type = Unit<U1::m - U2::m, U1::kg - U2::kg, U1::s - U2::s>;
};


template<typename U1, typename U2>
using UnitPlus = typename UPlus<U1, U2>::type;

template<typename U1, typename U2>
using UnitMinus = typename UMinus<U1, U2>::type;


template<typename U, typename T>
struct Quantity {
    T value;
    
    constexpr explicit Quantity(T val)
      : value{ val }
    {}
};

template<typename U, typename T>
Quantity<U, T> operator+(Quantity<U, T> lhs, Quantity<U, T> rhs)
{
    return Quantity<U, T>{ lhs.value + rhs.value };
}

template<typename U, typename T>
Quantity<U, T> operator-(Quantity<U, T> lhs, Quantity<U, T> rhs)
{
    return Quantity<U, T>{ lhs.value - rhs.value };
}

template<typename U1, typename U2, typename T>
Quantity<UnitPlus<U1, U2>, T> operator*(Quantity<U1, T> lhs, Quantity<U2, T> rhs)
{
    return Quantity<UnitPlus<U1, U2>, T>{ lhs.value * rhs.value };
}

template<typename U1, typename U2, typename T>
Quantity<UnitMinus<U1, U2>, T> operator/(Quantity<U1, T> lhs, Quantity<U2, T> rhs)
{
    return Quantity<UnitMinus<U1, U2>, T>{ lhs.value / rhs.value };
}

template<typename U, typename T, typename Scale>
    requires std::is_arithmetic_v<Scale>
Quantity<U, T> operator*(Quantity<U, T> quantity, Scale scaled)
{
    return Quantity<U, T>{ quantity.value * scaled };
}

template<typename U, typename T, typename Scale>
    requires std::is_arithmetic_v<Scale>
Quantity<U, T> operator*(Scale scaled, Quantity<U, T> quantity)
{
    return Quantity<U, T>{ quantity.value * scaled };
}

template<typename U, typename T, typename Scale>
    requires std::is_arithmetic_v<Scale>
Quantity<U, T> operator/(Quantity<U, T> quantity, Scale scaled)
{
    return Quantity<U, T>{ quantity.value / scaled };
}

template<typename U, typename T, T t>
Quantity<U, T> make_quantity()
{
    return Quantity<U, T>{ t };
}

template<typename U1, typename U2, typename T>
constexpr bool operator==(Quantity<U1, T> lhs, Quantity<U2, T> rhs)
{
    return lhs.value == rhs.value;
}

template<typename U1, typename U2, typename T>
constexpr bool operator!=(Quantity<U1, T> lhs, Quantity<U2, T> rhs)
{
    return lhs.value != rhs.value;
}

constexpr Quantity<M, long double> operator""_m(long double value) 
{ 
    return Quantity<M, long double>{ value }; 
}

constexpr Quantity<M, unsigned long long> operator""_m(unsigned long long value) 
{ 
    return Quantity<M, unsigned long long>{ value }; 
}

} // namespace SF

#endif // NEVERMORE_UNIT_H