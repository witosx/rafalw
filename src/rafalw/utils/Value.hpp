#ifndef RAFALW_UTILS_VALUE_HPP_
#define RAFALW_UTILS_VALUE_HPP_

#include <rafalw/typelist.hpp>
#include <type_traits>
#include <cmath>
#include <functional>

inline namespace rafalw {
namespace utils {

template<typename _Unit, typename _Quantity>
class Value
{
public:
    using Unit = _Unit;
    using Quantity = _Quantity;

    static_assert(std::is_arithmetic<Quantity>::value, "must be arithmetic type");

    constexpr Value() = default;

    constexpr explicit Value(Quantity q) :
        m_quantity{ q }
    {}

    template<typename Q2, typename = std::enable_if_t<std::is_convertible<Q2, Quantity>::value && (std::is_floating_point<Quantity>::value || !std::is_floating_point<Q2>::value)>>
    constexpr Value(Value<Unit, Q2> other) :
        Value{ static_cast<Quantity>(other.quantity()) }
    {}

    // properties

    constexpr auto quantity() const -> Quantity
    {
        return m_quantity;
    }

    // non-modyfying operators

    template<typename Q2, typename = std::enable_if_t<std::is_convertible<Q2, Quantity>::value>>
    constexpr explicit operator Value<Unit, Q2>()
    {
        return Value<Unit, Q2>{ static_cast<Q2>(m_quantity) };
    }

    constexpr auto operator +() const -> Value
    {
        return *this;
    }

    constexpr auto operator -() const -> Value
    {
        return Value{ -m_quantity };
    }

    // modyfying operators

    constexpr auto operator +=(Value o) -> Value&
    {
        m_quantity += o.quantity();
        return *this;
    }

    constexpr auto operator -=(Value o) -> Value&
    {
        m_quantity -= o.quantity();
        return *this;
    }

    constexpr auto operator *=(Quantity v) -> Value&
    {
        m_quantity *= v;
        return *this;
    }

    constexpr auto operator /=(Quantity v) -> Value&
    {
        m_quantity /= v;
        return *this;
    }

    constexpr auto operator %=(Quantity v) -> Value&
    {
        m_quantity %= v;
        return *this;
    }

private:
    Quantity m_quantity;
};

template<typename U, typename Q>
constexpr auto value(Q q) -> Value<U, Q>
{
    return Value<U, Q>{ q };
}

template<typename U, typename Q>
constexpr auto value_simplify(Value<U, Q> v) -> Value<U, Q>
{
    return v;
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


template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(Value<U, Q1> v, Q2 q) -> decltype(value<U>(v.quantity() * q))
{
    return value<U>(v.quantity() * q);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(Q2 q, Value<U, Q1> v) -> decltype(v * q)
{
    return v * q;
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator /(Value<U, Q1> v, Q2 q) -> decltype(value<U>(v.quantity() / q))
{
    return value<U>(v.quantity() / q);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator /(Value<U, Q1> v, Value<U, Q2> v2) -> decltype(v.quantity() / v2.quantity())
{
    return v.quantity() / v2.quantity();
}

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

} // namespace utils
} // namespace value

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
