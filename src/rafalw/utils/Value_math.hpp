#ifndef RAFALW_UTILS_VALUE_MATH_HPP_
#define RAFALW_UTILS_VALUE_MATH_HPP_

#include <rafalw/utils/Value.hpp>
#include <cmath>

inline namespace rafalw {
namespace utils {

template<typename U, typename Q>
auto abs(Value<U, Q> v) -> decltype(value<U>(std::abs(v.quantity())))
{
    return value<U>(std::abs(v.quantity()));
}

template<typename U, typename Q>
auto round(Value<U, Q> v) -> decltype(value<U>(std::round(v.quantity())))
{
    return value<U>(std::round(v.quantity()));
}

template<typename U, typename Q>
auto ceil(Value<U, Q> v) -> decltype(value<U>(std::ceil(v.quantity())))
{
    return value<U>(std::ceil(v.quantity()));
}

template<typename U, typename Q>
auto floor(Value<U, Q> v) -> decltype(value<U>(std::floor(v.quantity())))
{
    return value<U>(std::floor(v.quantity()));
}

template<typename U, typename Q>
auto sqrt(Value<U, Q> v) -> decltype(value<units::pow<U, std::ratio<1, 2>>>(std::sqrt(v.quantity())))
{
    return value<units::pow<U, std::ratio<1, 2>>>(std::sqrt(v.quantity()));
}

template<typename U, typename Q>
auto cbrt(Value<U, Q> v) -> decltype(value<units::pow<U, std::ratio<1, 3>>>(std::cbrt(v.quantity())))
{
    return value<units::pow<U, std::ratio<1, 3>>>(std::cbrt(v.quantity()));
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_VALUE_MATH_HPP_
