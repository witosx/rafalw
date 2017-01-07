#ifndef RAFALW_UTILS_VALUE_MATH_HPP_
#define RAFALW_UTILS_VALUE_MATH_HPP_

#include <rafalw/value/Value.hpp>
#include <cmath>

inline namespace rafalw {
namespace value {

template<typename U, typename Q>
auto abs(Value<U, Q> v) -> decltype(make<U>(std::abs(quantity(v))))
{
    return make<U>(std::abs(quantity(v)));
}

template<typename U, typename Q>
auto round(Value<U, Q> v) -> decltype(make<U>(std::round(quantity(v))))
{
    return make<U>(std::round(quantity(v)));
}

template<typename U, typename Q>
auto ceil(Value<U, Q> v) -> decltype(make<U>(std::ceil(quantity(v))))
{
    return make<U>(std::ceil(quantity(v)));
}

template<typename U, typename Q>
auto floor(Value<U, Q> v) -> decltype(make<U>(std::floor(quantity(v))))
{
    return make<U>(std::floor(quantity(v)));
}

template<typename U, typename Q>
auto sqrt(Value<U, Q> v) -> decltype(make<units::pow<U, std::ratio<1, 2>>>(std::sqrt(quantity(v))))
{
    return make<units::pow<U, std::ratio<1, 2>>>(std::sqrt(quantity(v)));
}

template<typename U, typename Q>
auto cbrt(Value<U, Q> v) -> decltype(make<units::pow<U, std::ratio<1, 3>>>(std::cbrt(quantity(v))))
{
    return make<units::pow<U, std::ratio<1, 3>>>(std::cbrt(quantity(v)));
}

} // namespace value
} // namespace rafalw

#endif // RAFALW_UTILS_VALUE_MATH_HPP_
