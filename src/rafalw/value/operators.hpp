#ifndef RAFALW_VALUE_OPERATORS_HPP_
#define RAFALW_VALUE_OPERATORS_HPP_

#include <rafalw/value/Value.hpp>
#include <type_traits>
#include <functional>

inline namespace rafalw {
namespace value {

namespace detail {

	template<typename T, typename std::enable_if_t<is_instance<T>>* = nullptr>
	constexpr auto valuize(T x) -> T
	{
		return x;
	}

	template<typename T, typename std::enable_if_t<!is_instance<T>>* = nullptr>
	constexpr auto valuize(T x) -> Value<units::Null, T>
	{
		return Value<units::Null, T>{ x };
	}

	template<typename Op, typename T1, typename T2>
	auto binary_operation(Op op, T1 lhs, T2 rhs) -> decltype(op(valuize(lhs), valuize(rhs)))
	{
		return op(valuize(lhs), valuize(rhs));
	}

} // namespace detail


// unary operators

template<typename U, typename Q>
constexpr auto operator +(Value<U, Q> v) -> Value<U, Q>
{
    return make<U>(+quantity(v));
}

template<typename U, typename Q>
constexpr auto operator -(Value<U, Q> v) -> Value<U, Q>
{
    return make<U>(-quantity(v));
}


// binary operators - comparison

template<typename U, typename Q1, typename Q2>
constexpr auto operator <(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) < quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator <=(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) <= quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator >(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) > quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator >=(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) >= quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator ==(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) == quantity(rhs);
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator !=(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return quantity(lhs) != quantity(rhs);
}


// binary operators - arithemtic

template<typename U, typename Q1, typename Q2>
constexpr auto operator +(Value<U, Q1> lhs, Value<U, Q2> rhs) -> decltype(make<U>(quantity(lhs) + quantity(rhs)))
{
    return make<U>(quantity(lhs) + quantity(rhs));
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator -(Value<U, Q1> lhs, Value<U, Q2> rhs) -> decltype(make<U>(quantity(lhs) - quantity(rhs)))
{
    return make<U>(quantity(lhs) - quantity(rhs));
}


template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator *(Value<U1, Q1> lhs, Value<U2, Q2> rhs) -> decltype(simplify(make<units::mul<U1, U2>>(quantity(lhs) * quantity(rhs))))
{
    return simplify(make<units::mul<U1, U2>>(quantity(lhs) * quantity(rhs)));
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(Value<U, Q1> lhs, Q2 rhs) -> decltype(detail::binary_operation(std::multiplies<>{}, lhs, rhs))
{
    return detail::binary_operation(std::multiplies<>{}, lhs, rhs);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(Q2 lhs, Value<U, Q1> rhs) -> decltype(detail::binary_operation(std::multiplies<>{}, lhs, rhs))
{
	return detail::binary_operation(std::multiplies<>{}, lhs, rhs);
}


template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator /(Value<U1, Q1> lhs, Value<U2, Q2> rhs) -> decltype(simplify(make<units::div<U1, U2>>(quantity(lhs) / quantity(rhs))))
{
    return simplify(make<units::div<U1, U2>>(quantity(lhs) / quantity(rhs)));
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator /(Value<U, Q1> lhs, Q2 rhs) -> decltype(detail::binary_operation(std::divides<>{}, lhs, rhs))
{
	return detail::binary_operation(std::divides<>{}, lhs, rhs);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator /(Q2 lhs, Value<U, Q1> rhs) -> decltype(detail::binary_operation(std::divides<>{}, lhs, rhs))
{
    return detail::binary_operation(std::divides<>{}, lhs, rhs);
}


template<typename U, typename Q>
constexpr auto operator +=(Value<U, Q>& lhs, Value<U, Q> rhs) -> Value<U, Q>&
{
    return lhs = lhs + rhs;;
}

template<typename U, typename Q>
constexpr auto operator -=(Value<U, Q>& lhs, Value<U, Q> rhs) -> Value<U, Q>&
{
    return lhs = lhs - rhs;
}

template<typename U, typename Q>
constexpr auto operator *=(Value<U, Q>& lhs, Q rhs) -> Value<U, Q>&
{
    return lhs = lhs * rhs;
}

template<typename U, typename Q>
constexpr auto operator /=(Value<U, Q>& lhs, Q rhs) -> Value<U, Q>&
{
    return lhs = lhs / rhs;
}

} // namespace value
} // namespace rafalw

#endif /* RAFALW_VALUE_OPERATORS_HPP_ */
