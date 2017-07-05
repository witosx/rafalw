#ifndef XTRAS_NUMERIC_ROLLINGSTATS_HPP_
#define XTRAS_NUMERIC_ROLLINGSTATS_HPP_

#include <xtras/utils/assert.hpp>
#include <deque>
#include <cmath>

namespace xtras {
namespace numeric {

template<typename ValueT, typename WeightT = double>
class RollingStats
{
public:
    using Value = ValueT;
    using Weight = WeightT;

    using Result = decltype(std::declval<Weight>() * std::declval<Value>());

    auto push(Value v, Weight w = 1) -> void
    {
        m_data.emplace_back(v, w);

        m_sumWxV += w * v;
        m_sumWxV2 += w * v * v;
        m_sumW += w;
    }

    auto popOldest() -> void
    {
        xtras_assert(!m_data.empty());

        const auto e = m_data.front();
        const auto v = e.first;
        const auto w = e.second;

        m_sumWxV -= w * v;
        m_sumWxV2 -= w * v * v;
        m_sumW -= w;

        m_data.pop_front();

        if (m_data.empty())
        {
            m_sumWxV = SumWxV{ 0 };
            m_sumWxV2 = SumWxV2{ 0 };
            m_sumW = Weight{ 0 };
        }
    }

    auto popNewest() -> void
    {
        xtras_assert(!m_data.empty());

        const auto e = m_data.back();
        const auto v = e.first;
        const auto w = e.second;

        m_sumWxV -= w * v;
        m_sumWxV2 -= w * v * v;
        m_sumW -= w;

        m_data.pop_back();

        if (m_data.empty())
        {
            m_sumWxV = SumWxV{ 0 };
            m_sumWxV2 = SumWxV2{ 0 };
            m_sumW = Weight{ 0 };
        }
    }

    auto pop() -> void
    {
        popOldest();
    }

    auto trim(std::size_t n) -> void
    {
        while (size() > n)
            popOldest();
    }

    auto size() const -> std::size_t
    {
        return m_data.size();
    }

    auto empty() const -> std::size_t
    {
        return m_data.empty();
    }

    auto sum() const -> Result
    {
        return m_sumWxV;
    }

    auto mean() const -> Result
    {
        xtras_assert(m_sumW != 0);
        return sum() / m_sumW;
    }

    auto stdev() const -> Result
    {
        using std::sqrt;

        xtras_assert(size() > 1);

        const auto n = size();
        const auto m = mean();
        const auto m2 = m * m;

        const auto x1 = m_sumWxV2;
        const auto x2 = 2 * m * m_sumWxV;
        const auto x3 = m2 * m_sumW;

        const auto d = m_sumW * (n - 1) / n;
        const auto var = (x1 - x2 + x3) / d;

        xtras_assert(var >= decltype(var){ 0 });

        return sqrt(var);
    }

private:
    using SumWxV = decltype(std::declval<Weight>() * std::declval<Value>());
    using SumWxV2 = decltype(std::declval<Weight>() * std::declval<Value>() * std::declval<Value>());
    using SumW = Weight;

    std::deque<std::pair<Value, Weight>> m_data;

    SumWxV m_sumWxV{ 0 };
    SumWxV2 m_sumWxV2{ 0 };
    SumW m_sumW{ 0 };
};

} // namespace numeric
} // namespace xtras

#endif // XTRAS_NUMERIC_ROLLINGSTATS_HPP_
