#ifndef RAFALW_NUMERIC_REAL_HPP_
#define RAFALW_NUMERIC_REAL_HPP_

#include <rafalw/utils/assert.hpp>
#include <cmath>
#include <limits>

inline namespace rafalw {
namespace numeric {

class Real
{
public:
    using ValueType = double;

    static constexpr auto NA() -> Real
    {
        return Real{};
    }

    constexpr Real() :
        m_value{ std::numeric_limits<ValueType>::quiet_NaN() }
    {}

    constexpr Real(ValueType v) :
        m_value{ v }
    {
        rafalw_utils_assert(!isNA());
    }

    constexpr auto value() const -> ValueType
    {
        rafalw_utils_assert(!isNA());
        return m_value;
    }

    constexpr auto value(ValueType v) const -> ValueType
    {
        if (isNA())
            return v;

        return value();
    }

    constexpr auto isNA() const -> bool
    {
        return std::isnan(m_value);
    }

    auto fillNA(Real v) const -> Real
    {
        if (isNA())
            return v;

        return *this;
    }

    auto operator +=(Real v) -> Real&
    {
        m_value += v.m_value;
        return *this;
    }

    auto operator -=(Real v) -> Real&
    {
        m_value -= v.m_value;
        return *this;
    }

    auto operator *=(Real v) -> Real&
    {
        m_value *= v.m_value;
        return *this;
    }

    auto operator /=(Real v) -> Real&
    {
        m_value /= v.m_value;
        return *this;
    }

private:
    ValueType m_value;
};

inline auto operator +(Real v1, Real v2) -> Real
{
    return v1 += v2;
}

inline auto operator -(Real v1, Real v2) -> Real
{
    return v1 -= v2;
}

inline auto operator *(Real v1, Real v2) -> Real
{
    return v1 *= v2;
}

inline auto operator /(Real v1, Real v2) -> Real
{
    return v1 /= v2;
}

inline auto operator <(Real v1, Real v2) -> bool
{
    return v1.value() < v2.value();
}

inline auto operator >(Real v1, Real v2) -> bool
{
    return v1.value() > v2.value();
}

inline auto is_na(Real arg) -> bool
{
    return arg.isNA();
}

template<typename... Args>
inline auto is_na(Real arg0, Args... args) -> bool
{
    return is_na(arg0) || is_na(args...);
}

inline auto abs(Real v) -> Real
{
    return std::abs(v.value());
}

inline auto ceil(Real v) -> Real
{
    return std::ceil(v.value());
}

inline auto floor(Real v) -> Real
{
    return std::floor(v.value());
}

inline auto round(Real v) -> Real
{
    return std::round(v.value());
}

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_REAL_HPP_
