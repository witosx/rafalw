#ifndef RAFALW_MATH_HPP_
#define RAFALW_MATH_HPP_

#include <rafalw/utils/assert.hpp>
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

template<typename T = int, typename = std::enable_if_t<std::is_integral<T>::value>>
constexpr auto int_pow2(int power) -> T
{
    rafalw_utils_assert(power >= 0);
    rafalw_utils_assert(power < static_cast<int>(sizeof(T)) * 8 - 1);
    return T{ 1 } << power;
}

} // namespace math
} // namespace rafalw

#endif // RAFALW_MATH_HPP_
