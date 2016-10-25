#ifndef RAFALW_MATH_HPP_
#define RAFALW_MATH_HPP_

#include <rafalw/utils/static.hpp>
#include <cmath>

inline namespace rafalw {
namespace math {

auto log(double base, double x) -> double
{
    return std::log2(x) / std::log2(base);
}

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr auto intdiv_round_up(T nom, T den) -> T
{
    return (nom - T{ 1 }) / den + T{ 1 };
}

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr auto intdiv_round_dn(T nom, T den) -> int
{
    return nom / den;
}

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr auto int_round_up(T v, T mult) -> T
{
    return intdiv_round_up(v, mult) * mult;
}

template<typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr auto int_round_dn(T v, T mult) -> int
{
    return intdiv_round_dn(v, mult) * mult;
}

}
}

#endif // RAFALW_MATH_HPP_
