#ifndef XTRAS_MATH_HPP_
#define XTRAS_MATH_HPP_

#include <xtras/utils/assert.hpp>
#include <xtras/utils/static.hpp>
#include <cmath>
#include <cstdint>

namespace xtras {
namespace math {

inline auto log(double base, double x) -> double
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
    xtras_assert(power >= 0);
    xtras_assert(power < static_cast<int>(sizeof(T)) * 8 - 1);
    return T{ 1 } << power;
}

template<typename Int = int, typename Float>
inline auto int_from_float(const Float input) -> Int
{
    auto intpart = Float{};

    if (std::modf(input, &intpart) != 0.0 ||
        intpart > std::numeric_limits<Int>::max() ||
        intpart < std::numeric_limits<Int>::min())
    {
        throw Error{ "floating point number ", input, " can't be treated as integral" };
    }

    return static_cast<Int>(intpart);
}

template<typename T1, typename T2>
constexpr auto round_dn(T1 v, T2 r) -> T1
{
    using std::floor;
    return static_cast<T1>(floor(v / r) * r);
}

template<typename T1, typename T2>
constexpr auto round_up(T1 v, T2 r) -> T1
{
    auto res = round_dn(v, r);

    if (res < v)
        res += r;

    return res;
}

} // namespace math
} // namespace xtras

#endif // XTRAS_MATH_HPP_
