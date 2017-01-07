#ifndef RAFALW_VALUE_VALUE_HPP_
#define RAFALW_VALUE_VALUE_HPP_

#include <rafalw/units.hpp>
#include <type_traits>

inline namespace rafalw {
namespace value {

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
	struct IsInstance : public std::false_type {};

	template<typename U, typename Q>
	struct IsInstance<Value<U, Q>> : public std::true_type {};

} // namespace detail

template<typename T>
using is_instance_t = typename detail::IsInstance<T>::type;

template<typename T>
constexpr auto is_instance = is_instance_t<T>::value;


template<typename U, typename Q>
constexpr auto make(Q q) -> Value<U, Q>
{
    return Value<U, Q>{ q };
}

template<typename U, typename Q>
constexpr auto quantity(Value<U, Q> v) -> Q
{
    return v.quantity();
}

template<typename Q2, typename U, typename Q1>
constexpr auto cast(Value<U, Q1> v) -> Value<U, Q2>
{
    return static_cast<Value<U, Q2>>(v);
}

namespace detail {

    template<typename T, typename std::enable_if_t<is_instance<T> && std::is_same<T, units::Null>::value>* = nullptr>
    constexpr auto simplify(T x) -> decltype(quantity(x))
    {
        return quantity(x);
    }

    template<typename T, typename std::enable_if_t<is_instance<T> && !std::is_same<T, units::Null>::value>* = nullptr>
	constexpr auto simplify(T x) -> T
	{
		return x;
	}

} // namespace detail


template<typename U, typename Q>
constexpr auto simplify(Value<U, Q> v) -> decltype(detail::simplify(v))
{
    return detail::simplify(v);
}

} // namespace value
} // namespace rafalw


namespace std {

template<typename U, typename Q>
struct hash<rafalw::value::Value<U, Q>>
{
    constexpr auto operator()(rafalw::value::Value<U, Q> v) const noexcept -> std::size_t
    {
        return hash<Q>{}(quantity(v));
    }
};

} // namespace std


#endif // RAFALW_VALUE_VALUE_HPP_
