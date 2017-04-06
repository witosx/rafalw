#ifndef RAFALW_NUMERIC_LMA_HPP_
#define RAFALW_NUMERIC_LMA_HPP_

#include <rafalw/utils/assert.hpp>
#include <deque>

inline namespace rafalw {
namespace numeric {

template<typename ValueT, typename WeightT = double>
class LMA
{
public:
    using Value = ValueT;
    using Weight = WeightT;

    struct Item
    {
        Value value;
        Weight weight;
    };

    using Items = std::deque<Item>;

private:
    using V = Value;
    using Wv = Weight;
    using Wf = decltype(std::declval<Items>().size());

    using WvV = decltype(std::declval<Wv>() * std::declval<V>());
    using WvWf = decltype(std::declval<Wv>() * std::declval<Wf>());
    using WvWfV = decltype(std::declval<Wv>() * std::declval<Wf>() * std::declval<V>());

    Wv m_sumWv{ 0 };
    WvV m_sumWvV{ 0 };
    WvWf m_sumWvWf{ 0 };
    WvWfV m_sumWvWfV{ 0 };

    Items m_items;

public:
    auto push(const Value value, const Weight weight = { 1 }) -> void
    {
        const auto v = value;
        const auto wv = weight;

        m_items.emplace_back(Item{ v, wv });

        const auto wf = m_items.size();

        m_sumWv += wv;
        m_sumWvV += wv * v;
        m_sumWvWf += wv * wf;
        m_sumWvWfV += wv * wf * v;
    }

    auto pop() -> void
    {
        rafalw_utils_assert(!m_items.empty());

        const auto v = m_items.front().value;
        const auto wv = m_items.front().weight;

        m_sumWvWfV -= m_sumWvV;
        m_sumWvWf -= m_sumWv;
        m_sumWvV -= wv * v;
        m_sumWv -= wv;

        m_items.pop_front();
    }

    auto value() const -> decltype(m_sumWvWfV / m_sumWvWf)
    {
        return m_sumWvWfV / m_sumWvWf;
    }

    auto items() const -> const Items&
    {
        return m_items;
    }

    auto totalWeight() const -> Weight
    {
        return m_sumWv;
    }
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_LMA_HPP_
