#ifndef XTRAS_VALUE_OPERATORS_HPP_
#define XTRAS_VALUE_OPERATORS_HPP_

#include <xtras/value/Value.hpp>
#include <type_traits>
#include <functional>

namespace xtras {
namespace value {

namespace detail {

	template<typename T, typename std::enable_if_t<is_instance<T>>* = nullptr>
	constexpr auto valuize(const T x) -> T
	{
		return x;
	}

	template<typename T, typename std::enable_if_t<!is_instance<T>>* = nullptr>
	constexpr auto valuize(const T x) -> Value<units::Null, T>
	{
		return Value<units::Null, T>{ x };
	}

	template<typename Op, typename T1, typename T2>
	auto binary_operation(const Op op, const T1 lhs, const T2 rhs) -> decltype(op(valuize(lhs), valuize(rhs)))
	{
		return op(valuize(lhs), valuize(rhs));
	}

} // namespace detail


// unary operators

template<typename U, typename Q>
constexpr auto operator +(const Value<U, Q> v) -> Value<U, Q>
{
    return make<U>(+quantity(v));
}

template<typename U, typename Q>
constexpr auto operator -(const Value<U, Q> v) -> Value<U, Q>
{
    return make<U>(-quantity(v));
}


// binary operators - comparison

template<typename U, typename Q1, typename Q2>
constexpr auto operator <(const Value<U, Q1> lhs, const Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) < quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator <=(const Value<U, Q1> lhs, const Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) <= quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator >(const Value<U, Q1> lhs, const Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) > quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator >=(const Value<U, Q1> lhs, const Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) >= quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator ==(const Value<U, Q1> lhs, const Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) == quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator !=(const Value<U, Q1> lhs, const Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) != quantity(rhs);
}


// binary operators - arithemtic

template<typename U, typename Q1, typename Q2>
constexpr auto operator +(const Value<U, Q1> lhs, const Value<U, Q2> rhs) -> decltype(make<U>(quantity(lhs) + quantity(rhs)))
{
    return make<U>(quantity(lhs) + quantity(rhs));
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator -(const Value<U, Q1> lhs, const Value<U, Q2> rhs) -> decltype(make<U>(quantity(lhs) - quantity(rhs)))
{
    return make<U>(quantity(lhs) - quantity(rhs));
}


template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator *(const Value<U1, Q1> lhs, const Value<U2, Q2> rhs) -> decltype(simplify(make<units::mul<U1, U2>>(quantity(lhs) * quantity(rhs))))
{
    return simplify(make<units::mul<U1, U2>>(quantity(lhs) * quantity(rhs)));
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(const Value<U, Q1> lhs, const Q2 rhs) -> decltype(detail::binary_operation(std::multiplies<>{}, lhs, rhs))
{
    return detail::binary_operation(std::multiplies<>{}, lhs, rhs);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(const Q2 lhs, const Value<U, Q1> rhs) -> decltype(detail::binary_operation(std::multiplies<>{}, lhs, rhs))
{
	return detail::binary_operation(std::multiplies<>{}, lhs, rhs);
}


template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator /(const Value<U1, Q1> lhs, const Value<U2, Q2> rhs) -> decltype(simplify(make<units::div<U1, U2>>(quantity(lhs) / quantity(rhs))))
{
    return simplify(make<units::div<U1, U2>>(quantity(lhs) / quantity(rhs)));
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator /(const Value<U, Q1> lhs, const Q2 rhs) -> decltype(detail::binary_operation(std::divides<>{}, lhs, rhs))
{
	return detail::binary_operation(std::divides<>{}, lhs, rhs);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator /(const Q2 lhs, const Value<U, Q1> rhs) -> decltype(detail::binary_operation(std::divides<>{}, lhs, rhs))
{
    return detail::binary_operation(std::divides<>{}, lhs, rhs);
}


template<typename U, typename Q>
constexpr auto operator +=(Value<U, Q>& lhs, const Value<U, Q> rhs) -> Value<U, Q>&
{
    return lhs = lhs + rhs;;
}

template<typename U, typename Q>
constexpr auto operator -=(Value<U, Q>& lhs, const Value<U, Q> rhs) -> Value<U, Q>&
{
    return lhs = lhs - rhs;
}

template<typename U, typename Q>
constexpr auto operator *=(Value<U, Q>& lhs, const Q rhs) -> Value<U, Q>&
{
    return lhs = lhs * rhs;
}

template<typename U, typename Q>
constexpr auto operator /=(Value<U, Q>& lhs, const Q rhs) -> Value<U, Q>&
{
    return lhs = lhs / rhs;
}

} // namespace value
} // namespace xtras

#endif /* XTRAS_VALUE_OPERATORS_HPP_ */
