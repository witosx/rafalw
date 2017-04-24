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

    auto generatorReset()
    {
        m_current = m_begin;
        return RESET_OK;
    }
};


template<typename IteratorT, typename IteratorEndT>
auto view(IteratorT begin, IteratorEndT end) -> View<IteratorT, IteratorEndT>
{
    return View<IteratorT, IteratorEndT>{ std::move(begin), std::move(end) };
}

template<typename RangeT>
auto view(RangeT&& range)
{
    using std::begin;
    using std::end;
    return view(begin(range), end(range));
}

template<typename RangeT>
auto reversed_view(RangeT&& range)
{
    using std::rbegin;
    using std::rend;
    return view(rbegin(range), rend(range));
}

template<typename RangeT>
auto const_view(RangeT&& range) -> decltype(view(static_cast<std::add_const_t<std::remove_reference_t<RangeT>>&>(range)))
{
    return view(static_cast<std::add_const_t<std::remove_reference_t<RangeT>>&>(range));
}

template<typename RangeT>
auto const_reversed_view(RangeT&& range) -> decltype(reversed_view(static_cast<std::add_const_t<std::remove_reference_t<RangeT>>&>(range)))
{
    return reversed_view(static_cast<std::add_const_t<std::remove_reference_t<RangeT>>&>(range));
}


} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_VIEW_HPP_

