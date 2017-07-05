#ifndef XTRAS_UTILS_VALUE_MATH_HPP_
#define XTRAS_UTILS_VALUE_MATH_HPP_

#include <xtras/value/Value.hpp>
#include <cmath>

namespace xtras {
namespace value {

template<typename U, typename Q>
auto abs(const Value<U, Q> v) -> decltype(make<U>(std::abs(quantity(v))))
{
    return make<U>(std::abs(quantity(v)));
}

template<typename U, typename Q>
auto round(const Value<U, Q> v) -> decltype(make<U>(std::round(quantity(v))))
{
    return make<U>(std::round(quantity(v)));
}

template<typename U, typename Q>
auto ceil(const Value<U, Q> v) -> decltype(make<U>(std::ceil(quantity(v))))
{
    return make<U>(std::ceil(quantity(v)));
}

template<typename U, typename Q>
auto floor(const Value<U, Q> v) -> decltype(make<U>(std::floor(quantity(v))))
{
    return make<U>(std::floor(quantity(v)));
}

template<typename U, typename Q>
auto sqrt(const Value<U, Q> v) -> decltype(make<units::pow<U, std::ratio<1, 2>>>(std::sqrt(quantity(v))))
{
    return make<units::pow<U, std::ratio<1, 2>>>(std::sqrt(quantity(v)));
}

template<typename U, typename Q>
auto cbrt(const Value<U, Q> v) -> decltype(make<units::pow<U, std::ratio<1, 3>>>(std::cbrt(quantity(v))))
{
    return make<units::pow<U, std::ratio<1, 3>>>(std::cbrt(quantity(v)));
}

} // namespace value
} // namespace xtras

#endif // XTRAS_UTILS_VALUE_MATH_HPP_
