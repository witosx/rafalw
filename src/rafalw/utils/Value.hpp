#ifndef RAFALW_UTILS_VALUE_HPP_
#define RAFALW_UTILS_VALUE_HPP_

#include <rafalw/units.hpp>
#include <type_traits>
#include <functional>

inline namespace rafalw {
namespace utils {

template<typename UnitT, typename QuantityT>
class Value;

template<typename... UnitsT, typename QuantityT>
class Value<units::UnitSet<UnitsT...>, QuantityT>
{
public:
    using Unit = units::UnitSet<UnitsT...>;
    using Quantity = QuantityT;

    static_assert(std::is_arithmetic<Quantity>::value, "must be arithmetic type");

    constexpr Value() = default;

    constexpr explicit Value(Quantity q) :
        m_quantity{ q }
    {}

    template<typename Q2, typename = std::enable_if_t<std::is_convertible<Q2, Quantity>::value && (std::is_floating_point<Quantity>::value || !std::is_floating_point<Q2>::value)>>
    constexpr Value(Value<Unit, Q2> other) :
        Value{ static_cast<Quantity>(other.quantity()) }
    {}

    template<typename Q2, typename = std::enable_if_t<std::is_convertible<Q2, Quantity>::value>>
    constexpr explicit operator Value<Unit, Q2>() const
    {
        return Value<Unit, Q2>{ static_cast<Q2>(quantity()) };
    }

    constexpr auto quantity() const -> Quantity
    {
        return m_quantity;
    }

private:
    Quantity m_quantity;
};

namespace detail {

	template<typename T>
	struct IsValue : public std::false_type {};

	template<typename U, typename Q>
	struct IsValue<Value<U, Q>> : public std::true_type {};

}

template<typename T>
using is_value_t = typename detail::IsValue<T>::type;

template<typename T>
constexpr auto is_value = is_value_t<T>::value;


namespace detail {

    template<typename V>
    constexpr auto simplify(V v, std::false_type) -> V
    {
        return v;
    }

    template<typename V>
    constexpr auto simplify(V v, std::true_type) -> decltype(v.quantity())
    {
        return v.quantity();
    }

    template<typename T, typename std::enable_if_t<is_value<T>>* = nullptr>
	constexpr auto valuize(T x) -> T
	{
		return x;
	}

    template<typename T, typename std::enable_if_t<!is_value<T>>* = nullptr>
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


template<typename U, typename Q>
constexpr auto value(Q q) -> Value<U, Q>
{
    return Value<U, Q>{ q };
}

template<typename Q2, typename U, typename Q1>
constexpr auto value_cast(Value<U, Q1> v) -> Value<U, Q2>
{
    return static_cast<Value<U, Q2>>(v);
}

template<typename U, typename Q>
constexpr auto value_simplify(Value<U, Q> v) -> decltype(detail::simplify(v, typename std::is_same<U, units::Null>::type{}))
{
    return detail::simplify(v, typename std::is_same<U, units::Null>::type{});
}


// unary operators

template<typename U, typename Q>
constexpr auto operator +(Value<U, Q> v) -> Value<U, Q>
{
    return value<U>(+v.quantity());
}

template<typename U, typename Q>
constexpr auto operator -(Value<U, Q> v) -> Value<U, Q>
{
    return value<U>(-v.quantity());
}

// binary operators - comparison

template<typename U, typename Q1, typename Q2>
constexpr auto operator <(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return lhs.quantity() < rhs.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator <=(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return lhs.quantity() <= rhs.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator >(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return lhs.quantity() > rhs.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator >=(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return lhs.quantity() >= rhs.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator ==(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return lhs.quantity() == rhs.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator !=(Value<U, Q1> lhs, Value<U, Q2> rhs) -> bool
{
    return lhs.quantity() != rhs.quantity();
}

// binary operators - arithemtic

template<typename U, typename Q1, typename Q2>
constexpr auto operator +(Value<U, Q1> lhs, Value<U, Q2> rhs) -> decltype(value<U>(lhs.quantity() + rhs.quantity()))
{
    return value<U>(lhs.quantity() + rhs.quantity());
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator -(Value<U, Q1> lhs, Value<U, Q2> rhs) -> decltype(value<U>(lhs.quantity() - rhs.quantity()))
{
    return value<U>(lhs.quantity() - rhs.quantity());
}

template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator *(Value<U1, Q1> lhs, Value<U2, Q2> rhs) -> decltype(value_simplify(value<units::mul<U1, U2>>(lhs.quantity() * rhs.quantity())))
{
    return value_simplify(value<units::mul<U1, U2>>(lhs.quantity() * rhs.quantity()));
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
constexpr auto operator /(Value<U1, Q1> lhs, Value<U2, Q2> rhs) -> decltype(value_simplify(value<units::div<U1, U2>>(lhs.quantity() / rhs.quantity())))
{
    return value_simplify(value<units::div<U1, U2>>(lhs.quantity() / rhs.quantity()));
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

} // namespace utils
} // namespace rafalw

namespace std {

template<typename U, typename Q>
struct hash<rafalw::utils::Value<U, Q>>
{
    constexpr auto operator()(rafalw::utils::Value<U, Q> v) const noexcept -> std::size_t
    {
        return hash<Q>{}(v.quantity());
    }
};

} // namespace std


#endif // RAFALW_UTILS_VALUE_HPP_
