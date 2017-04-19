#ifndef RAFALW_GENERATOR_WRAP_HPP_
#define RAFALW_GENERATOR_WRAP_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename IteratorT, typename IteratorEndT = IteratorT>
class Wrap : private Base
{
public:
    using Iterator = IteratorT;
    using IteratorEnd = IteratorEndT;

    Wrap(const Iterator begin, const IteratorEnd end) :
        m_current{ begin },
        m_end{ end }
    {}

private:
    friend class BaseAccess;

    Iterator m_current;
    IteratorEnd m_end;

    auto generatorDone() const -> bool
    {
        return m_current == m_end;
    }

    decltype(auto) generatorPeek() const
    {
        return *m_current;
    }

    auto generatorUpdate() -> void
    {
        m_current++;
    }
};

template<typename IteratorT, typename IteratorEndT>
auto wrap(IteratorT begin, IteratorEndT end) -> Wrap<IteratorT, IteratorEndT>
{
    return Wrap<IteratorT, IteratorEndT>{ begin, end };
}

template<typename RangeT>
auto wrap(RangeT&& range) -> Wrap<decltype(begin(range)), decltype(end(range))>
{
    return Wrap<decltype(begin(range)), decltype(end(range))>{ begin(range), end(range) };
}

template<typename RangeT>
auto rwrap(RangeT&& range) -> Wrap<decltype(rbegin(range)), decltype(rend(range))>
{
    return Wrap<decltype(rbegin(range)), decltype(rend(range))>{ rbegin(range), rend(range) };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_WRAP_HPP_

