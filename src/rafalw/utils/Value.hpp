#ifndef RAFALW_UTILS_VALUE_HPP_
#define RAFALW_UTILS_VALUE_HPP_

#include <rafalw/units.hpp>
#include <type_traits>
#include <cmath>
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
    constexpr explicit operator Value<Unit, Q2>()
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

// non-modyfying operators

template<typename U, typename Q>
constexpr auto value(Q q) -> Value<U, Q>
{
    return Value<U, Q>{ q };
}

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

    template<typename U, typename Q>
    constexpr auto simplify(Value<U, Q> v) -> decltype(simplify(v, typename std::is_same<U, units::Null>::type{}))
    {
        return simplify(v, typename std::is_same<U, units::Null>::type{});
    }

} // namespace detail


template<typename U, typename Q>
constexpr auto operator +(Value<U, Q> v) -> Value<U, Q>
{
    return v;
}

template<typename U, typename Q>
constexpr auto operator -(Value<U, Q> v) -> Value<U, Q>
{
    return value<U>(-v.quantity());
}

// binary operators - comparison

template<typename U, typename Q1, typename Q2>
constexpr auto operator <(Value<U, Q1> v1, Value<U, Q2> v2) -> bool
{
    return v1.quantity() < v2.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator <=(Value<U, Q1> v1, Value<U, Q2> v2) -> bool
{
    return v1.quantity() <= v2.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator >(Value<U, Q1> v1, Value<U, Q2> v2) -> bool
{
    return v1.quantity() > v2.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator >=(Value<U, Q1> v1, Value<U, Q2> v2) -> bool
{
    return v1.quantity() >= v2.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator ==(Value<U, Q1> v1, Value<U, Q2> v2) -> bool
{
    return v1.quantity() == v2.quantity();
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator !=(Value<U, Q1> v1, Value<U, Q2> v2) -> bool
{
    return v1.quantity() != v2.quantity();
}

// binary operators - arithemtic

template<typename U, typename Q1, typename Q2>
constexpr auto operator +(Value<U, Q1> v1, Value<U, Q2> v2) -> decltype(value<U>(v1.quantity() + v2.quantity()))
{
    return value<U>(v1.quantity() + v2.quantity());
}

template<typename U, typename Q1, typename Q2>
constexpr auto operator -(Value<U, Q1> v1, Value<U, Q2> v2) -> decltype(value<U>(v1.quantity() - v2.quantity()))
{
    return value<U>(v1.quantity() - v2.quantity());
}

template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator *(Value<U1, Q1> v1, Value<U2, Q2> v2) -> decltype(detail::simplify(value<units::mul<U1, U2>>(v1.quantity() * v2.quantity())))
{
    return detail::simplify(value<units::mul<U1, U2>>(v1.quantity() * v2.quantity()));
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(Value<U, Q1> v, Q2 q) -> decltype(value<U>(v.quantity() * q))
{
    return v * value<units::Null>(q);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(Q2 q, Value<U, Q1> v) -> decltype(v * q)
{
    return value<units::Null>(q) * v;
}

template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator /(Value<U1, Q1> v1, Value<U2, Q2> v2) -> decltype(detail::simplify(value<units::div<U1, U2>>(v1.quantity() * v2.quantity())))
{
    return detail::simplify(value<units::div<U1, U2>>(v1.quantity() * v2.quantity()));
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator /(Value<U, Q1> v, Q2 q) -> decltype(value<U>(v.quantity() / q))
{
    return v / value<units::Null>(q);
}


template<typename U, typename Q>
constexpr auto operator +=(Value<U, Q>& v1, Value<U, Q> v2) -> Value<U, Q>&
{
    v1 = v1 + v2;
    return v1;
}

template<typename U, typename Q>
constexpr auto operator -=(Value<U, Q>& v1, Value<U, Q> v2) -> Value<U, Q>&
{
    v1 = v1 - v2;
    return v1;
}

template<typename U, typename Q>
constexpr auto operator *=(Value<U, Q>& v1, Q q) -> Value<U, Q>&
{
    v1 = v1 * q;
    return v1;
}

template<typename U, typename Q>
constexpr auto operator /=(Value<U, Q>& v1, Q q) -> Value<U, Q>&
{
    v1 = v1 / q;
    return v1;
}

//template<typename U, typename Q>
//constexpr auto operator /=(Value2<U, Q>& v1, Q q) -> Value2<U, Q>&
//{
//    v1 = v1  q;
//    return v1;
//}

// various mathematical functions

template<typename U, typename Q>
auto abs(Value<U, Q> v) -> Value<U, Q>
{
    return value<U>(std::abs(v.quantity()));
}

template<typename U, typename Q>
auto round(Value<U, Q> v) -> Value<U, Q>
{
    return value<U>(std::round(v.quantity()));
}

template<typename U, typename Q>
auto ceil(Value<U, Q> v) -> Value<U, Q>
{
    return value<U>(std::ceil(v.quantity()));
}

template<typename U, typename Q>
auto floor(Value<U, Q> v) -> Value<U, Q>
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

namespace std {

template<typename U, typename Q>
struct hash<rafalw::utils::Value<U, Q>>
{
    constexpr auto operator()(rafalw::utils::Value<U, Q> v) const
    {
        auto q = v.quantity();
        return std::hash<decltype(q)>()(q);
    }
};

} // namespace std


#endif // RAFALW_UTILS_VALUE_HPP_
