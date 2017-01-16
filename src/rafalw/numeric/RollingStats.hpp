#ifndef RAFALW_NUMERIC_ROLLINGSTATS_HPP_
#define RAFALW_NUMERIC_ROLLINGSTATS_HPP_

#include <rafalw/utils/assert.hpp>
#include <deque>
#include <cmath>

inline namespace rafalw {
namespace numeric {

template<typename ValueT>
class RollingStats
{
public:
    using Value = ValueT;

    auto push(Value v) -> void
    {
        m_buffer.push_back(v);
        m_sum1 += v;
        m_sum2 += v * v;
    }

    auto pop() -> void
    {
        rafalw_utils_assert(!m_buffer.empty());
        auto v = m_buffer.front();
        m_sum1 -= v;
        m_sum2 -= v * v;
        m_buffer.pop_front();
    }

    auto size() const -> std::size_t
    {
        return m_buffer.size();
    }

    auto empty() const -> std::size_t
    {
        return m_buffer.empty();
    }

    auto mean() const -> Value
    {
        rafalw_utils_assert(size() > 0);
        return m_sum1 / size();
    }

    auto stdev() const -> Value
    {
        using std::sqrt;

        rafalw_utils_assert(size() > 1);
        const auto n = size();
        const auto m1 = mean();
        const auto m2 = m1 * m1;
        return sqrt(m_sum2 / (n - 1) - 2 * m1 * m_sum1 / (n - 1) + n * m2 / (n - 1));
    }

private:
    using Value2 = decltype(std::declval<Value>() * std::declval<Value>());

    std::deque<Value> m_buffer;

    Value m_sum1{ 0 };
    Value2 m_sum2{ 0 };
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_ROLLINGSTATS_HPP_
