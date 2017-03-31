#ifndef RAFALW_VALUE_VALUE_HPP_
#define RAFALW_VALUE_VALUE_HPP_

#include <rafalw/units.hpp>
#include <type_traits>

inline namespace rafalw {
namespace value {

template<typename UnitT, typename QuantityT>
class Value
{
private:
    using Unit = UnitT;
    using Quantity = QuantityT;

public:
    static_assert(units::is_instance<Unit>);
    static_assert(std::is_arithmetic<Quantity>::value);

    constexpr Value() = default;

    constexpr explicit Value(const Quantity q) :
        m_quantity{ q }
    {}

    template<typename Q2, typename = std::enable_if_t<std::is_convertible<Q2, Quantity>::value && (std::is_floating_point<Quantity>::value || !std::is_floating_point<Q2>::value)>>
    constexpr Value(const Value<Unit, Q2> other) :
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

	template<typename T>
	struct QuantityT;

	template<typename U, typename Q>
	struct QuantityT<Value<U, Q>>
	{
		using Result = Q;
	};

	template<typename T>
	struct UnitT;

	template<typename U, typename Q>
	struct UnitT<Value<U, Q>>
	{
		using Result = U;
	};

} // namespace detail

template<typename T>
using unit_t = typename detail::UnitT<T>::Result;

template<typename T>
using quantity_t = typename detail::QuantityT<T>::Result;


template<typename T>
using is_instance_t = typename detail::IsInstance<T>::type;

template<typename T>
constexpr auto is_instance = is_instance_t<T>::value;


template<typename U, typename Q>
constexpr auto make(const Q q) -> Value<U, Q>
{
    return Value<U, Q>{ q };
}

template<typename U, typename Q>
constexpr auto quantity(const Value<U, Q> v) -> Q
{
    return v.quantity();
}

template<typename Q2, typename U, typename Q1>
constexpr auto cast(const Value<U, Q1> v) -> Value<U, Q2>
{
    return static_cast<Value<U, Q2>>(v);
}

namespace detail {

    template<typename T, typename std::enable_if_t<units::is_null<unit_t<T>>>* = nullptr>
    constexpr auto simplify(const T x) -> decltype(quantity(x))
    {
        return quantity(x);
    }

    template<typename T, typename std::enable_if_t<!units::is_null<unit_t<T>>>* = nullptr>
	constexpr auto simplify(const T x) -> T
	{
		return x;
	}

} // namespace detail


template<typename U, typename Q>
constexpr auto simplify(const Value<U, Q> v) -> decltype(detail::simplify(v))
{
    return detail::simplify(v);
}

} // namespace value
} // namespace rafalw


namespace std {

template<typename U, typename Q>
struct hash<rafalw::value::Value<U, Q>>
{
    constexpr auto operator()(const rafalw::value::Value<U, Q> v) const noexcept -> std::size_t
    {
        return hash<Q>{}(quantity(v));
    }
};

} // namespace std


#endif // RAFALW_VALUE_VALUE_HPP_
