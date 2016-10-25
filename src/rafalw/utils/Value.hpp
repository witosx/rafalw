#ifndef RAFALW_UTILS_VALUE_HPP_
#define RAFALW_UTILS_VALUE_HPP_

#include <rafalw/typelist.hpp>
#include <type_traits>
#include <cmath>
#include <functional>

inline namespace rafalw {
namespace utils {

template<typename N = typelist::empty, typename D = typelist::empty>
struct ValueUnit
{
    using Numerator = N;
    using Denominator = D;
};

namespace detail {

    template<typename U>
    struct ValueUnitSimplify
    {
        using Unit = U;
    };

    template<typename U>
    struct ValueUnitSimplify<ValueUnit<typelist::List<U>, typelist::empty>>
    {
        using Unit = U;
    };

    template<typename E>
    struct ValueUnitAsComplex
    {
        using Unit = ValueUnit<typelist::List<E>, typelist::empty>;
    };

    template<typename N, typename D>
    struct ValueUnitAsComplex<ValueUnit<N, D>>
    {
        using Unit = ValueUnit<N, D>;
    };


    template<typename L, typename Idx>
    struct ValueUnitNormalizeErase;

    template<typename L>
    struct ValueUnitNormalizeErase<L, typelist::NotFound>
    {
        using Result = L;
    };

    template<typename L, std::size_t I>
    struct ValueUnitNormalizeErase<L, typelist::Index<I>>
    {
        using Result = typelist::erase<L, I>;
    };

    template<typename L1, typename L2>
    struct ValueUnitNormalize
    {
        using E = typelist::head<L2>;
        using Idx = typelist::find<L1, E>;
        using L1b = typename ValueUnitNormalizeErase<L1, Idx>::Result;
        using L2b = typelist::tail<L2>;
        using Result = typename ValueUnitNormalize<L1b, L2b>::Result;
    };

    template<typename L1>
    struct ValueUnitNormalize<L1, typelist::empty>
    {
        using Result = L1;
    };

    template<typename U>
    struct ValueUnitInvert
    {
        using C = typename ValueUnitAsComplex<U>::Unit;
        using N = typename C::Numerator;
        using D = typename C::Denominator;
        using Unit = ValueUnit<D, N>;
    };

    template<typename U1, typename U2>
    struct ValueUnitMultiply
    {
        using C1 = typename ValueUnitAsComplex<U1>::Unit;
        using C2 = typename ValueUnitAsComplex<U2>::Unit;

        using N = typelist::concat<typename C1::Numerator, typename C2::Numerator>;
        using D = typelist::concat<typename C1::Denominator, typename C2::Denominator>;

        using N2 = typename ValueUnitNormalize<N, D>::Result;
        using D2 = typename ValueUnitNormalize<D, N>::Result;

        using Unit = typename ValueUnitSimplify<ValueUnit<N2, D2>>::Unit;
    };

    template<typename U1, typename U2>
    using ValueUnitMultiplyT = typename ValueUnitMultiply<U1, U2>::Unit;
}

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

template<typename Q>
constexpr auto value_simplify(Value<ValueUnit<typelist::empty, typelist::empty>, Q> v) -> Q
{
    return v.quantity();
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


template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator *(Value<U1, Q1> v1, Value<U2, Q2> v2) -> decltype(value_simplify(value<detail::ValueUnitMultiplyT<U1, U2>>(v1.quantity() * v2.quantity())))
{
    return value_simplify(value<detail::ValueUnitMultiplyT<U1, U2>>(v1.quantity() * v2.quantity()));
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(Value<U, Q1> v, Q2 q) -> decltype(value<U>(v.quantity() * q))
{
    return v * value<ValueUnit<typelist::empty, typelist::empty>>(q);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator *(Q2 q, Value<U, Q1> v) -> decltype(v * q)
{
    return v * q;
}



template<typename U1, typename Q1, typename U2, typename Q2>
constexpr auto operator /(Value<U1, Q1> v1, Value<U2, Q2> v2) -> decltype(v1 * value<typename detail::ValueUnitInvert<U2>::Unit>(1 / v2.quantity()))
{
    return v1 * value<typename detail::ValueUnitInvert<U2>::Unit>(Q1{ 1 } / v2.quantity());
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q2>::value>>
constexpr auto operator /(Value<U, Q1> v, Q2 q) -> decltype(value<U>(v.quantity() / q))
{
    return v / value<ValueUnit<>>(q);
}

template<typename U, typename Q1, typename Q2, typename = std::enable_if_t<std::is_arithmetic<Q1>::value>>
constexpr auto operator /(Q1 q, Value<U, Q2> v) -> decltype(value<ValueUnit<>>(q) / v)
{
    return value<ValueUnit<>>(q) / v;
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
