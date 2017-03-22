#ifndef RAFALW_NUMERIC_SMA_HPP_
#define RAFALW_NUMERIC_SMA_HPP_

#include <rafalw/numeric/EMA1.hpp>
#include <rafalw/numeric/EMA2A.hpp>
#include <rafalw/numeric/EMA3.hpp>
#include <boost/variant.hpp>

inline namespace rafalw {
namespace numeric {

template<typename ValueT, typename WeightT = double>
class SMA
{
public:
    using Value = ValueT;
    using Weight = WeightT;

    using Result = decltype(std::declval<Weight>() * std::declval<Value>());

    struct Item
    {
        Value value;
        Weight weight;
    };

    using Data = std::deque<Item>;

    auto pushElement(Value v, Weight w = Weight{ 1 }) -> void
    {
        m_data.emplace_back(Item{ v, w });
        m_sumWxV += w * v;
        m_sumW += w;
    }

    auto pushWeight(Weight w) -> void
    {
        rafalw_utils_assert(!m_data.empty());

        const auto v = m_data.back().value;

        m_data.back().weight += w;
        m_sumWxV += w * v;
        m_sumW += w;
    }

    auto popElement() -> void
    {
        rafalw_utils_assert(!m_data.empty());

        const auto e = m_data.front();
        const auto v = e.value;
        const auto w = e.weight;

        m_sumWxV -= w * v;
        m_sumW -= w;
        m_data.pop_front();

        if (m_data.empty())
        {
            m_sumWxV = SumWxV{ 0 };
            m_sumW = Weight{ 0 };
        }
    }

    auto popWeight(Weight w) -> void
    {
        while (!m_data.empty() && !(m_data.front().weight > w))
        {
            w -= m_data.front().weight;
            popElement();
        }

        rafalw_utils_assert(!m_data.empty());

        auto& e = m_data.front();

        e.weight -= w;
        m_sumWxV -= w * e.value;
        m_sumW -= w;
    }

    auto trimCount(std::size_t n) -> void
    {
        while (size() > n)
            popElement();
    }

    auto trimWeight(Weight w) -> void
    {
        if (totalWeight() > w)
            popWeight(totalWeight() - w);
    }

    auto value() const -> Result
    {
        rafalw_utils_assert(totalWeight() != 0);
        return totalValue() / totalWeight();
    }

    auto totalValue() const -> Result
    {
        return m_sumWxV;
    }

    auto totalWeight() const -> Weight
    {
        return m_sumW;
    }

    // access to the data

    auto data() const -> const Data&
    {
        return m_data;
    }

    auto size() const -> std::size_t
    {
        return data().size();
    }

    auto empty() const -> std::size_t
    {
        return data().empty();
    }

    auto front() const -> const Item&
    {
        return data().front();
    }

    auto back() const -> const Item&
    {
        return data().back();
    }

private:
    using SumWxV = decltype(std::declval<Weight>() * std::declval<Value>());
    using SumW = Weight;

    Data m_data;
    SumWxV m_sumWxV{ 0 };
    SumW m_sumW{ 0 };
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_SMA_HPP_
