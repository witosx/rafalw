#ifndef RAFALW_GENERATOR_VIEW_HPP_
#define RAFALW_GENERATOR_VIEW_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename IteratorT, typename IteratorEndT = IteratorT>
class View : private Base
{
public:
    using Iterator = IteratorT;
    using IteratorEnd = IteratorEndT;

    View(const Iterator begin, const IteratorEnd end) :
        m_begin{ begin },
        m_end{ end }
    {}

private:
    friend class BaseAccess;

    Iterator m_begin;
    IteratorEnd m_end;

    Iterator m_current = m_begin;

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

    auto generatorReset() -> void
    {
        m_current = m_begin;
    }
};

template<typename IteratorT, typename IteratorEndT>
auto view(IteratorT begin, IteratorEndT end) -> View<IteratorT, IteratorEndT>
{
    return View<IteratorT, IteratorEndT>{ begin, end };
}

template<typename RangeT>
auto view(const RangeT& range) -> decltype(view(begin(range), end(range)))
{
    return view(begin(range), end(range));
}

template<typename RangeT>
auto rview(const RangeT& range) -> decltype(view(rbegin(range), rend(range)))
{
    return view(rbegin(range), rend(range));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_VIEW_HPP_

