#ifndef RAFALW_NUMERIC_ROLLINGMINMAX_HPP_
#define RAFALW_NUMERIC_ROLLINGMINMAX_HPP_

#include <rafalw/utils/assert.hpp>
#include <deque>
#include <functional>
#include <limits>
#include <type_traits>
#include <utility>

inline namespace rafalw {
namespace numeric {

template<typename _Index, typename _Value, typename _Compare>
class RollingMinMax
{
public:
    using Index = _Index;
    using Value = _Value;
    using Size = std::size_t;

private:
    using Compare = _Compare;
    using WindowBuffer = std::deque<std::pair<Index, Value>>;

    WindowBuffer m_buffer;
    Compare m_compare;

public:
    template<typename T = Compare, typename = std::enable_if_t<std::is_default_constructible<T>::value>>
    RollingMinMax() :
        RollingMinMax{ Compare{} }
    {}

    RollingMinMax(Compare cmp) :
        m_compare{ cmp }
    {}

    auto push(Index index, Value value) -> void
    {
        while (!m_buffer.empty() && !m_compare(m_buffer.back().second, value))
            m_buffer.pop_back();

        m_buffer.push_back(std::make_pair(index, value));
    }

    auto trim(Index index) -> void
    {
        while (!m_buffer.empty() && m_buffer.front().first < index)
            m_buffer.pop_front();
    }

    auto size() const -> Size
    {
        return m_buffer.size();
    }

    auto empty() const -> bool
    {
        return m_buffer.empty();
    }

    auto index() const -> Index
    {
        rafalw_utils_assert(!empty());
        return m_buffer.front().first;
    }

    auto value() const -> Value
    {
        rafalw_utils_assert(!empty());
        return m_buffer.front().second;
    }
};

template<typename Index, typename Value>
using RollingMin = RollingMinMax<Index, Value, std::less<Value>>;

template<typename Index, typename Value>
using RollingMax = RollingMinMax<Index, Value, std::greater<Value>>;

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_ROLLINGMINMAX_HPP_
