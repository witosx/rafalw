#ifndef RAFALW_UTILS_GENERATOR_HPP_
#define RAFALW_UTILS_GENERATOR_HPP_

#include <rafalw/utils/ScopeGuard.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <cassert>

inline namespace rafalw {
namespace utils {

template<typename _Derived, typename _Element>
class Generator
{
public:
    using Element = _Element;

    class Iterator;

    auto next() -> Element
    {
        auto deferred_update = utils::scope_guard([this](){
            get().update();
        });

        return get().peek();
    }

    auto begin() -> Iterator
    {
        return Iterator{ &get() };
    }

    auto end() -> Iterator
    {
        return Iterator{ nullptr };
    }

    operator bool() const
    {
        return !get().done();
    }

    auto operator !() const -> bool
    {
        return get().done();
    }

    auto operator ()() -> Element
    {
        return next();
    }

private:
    using Derived = _Derived;

    auto get() -> Derived&
    {
        return static_cast<Derived&>(*this);
    }

    auto get() const -> const Derived&
    {
        return static_cast<const Derived&>(*this);
    }
};

template<typename T, typename E>
class Generator<T, E>::Iterator :
    public boost::iterator_facade<Iterator, const Element, boost::single_pass_traversal_tag>
{
private:
    friend class boost::iterator_core_access;
    friend class Generator;

    T* m_generator;
    bool m_empty;

    explicit Iterator(T* g) :
        m_generator{ g },
        m_empty{ false }
    {
        if (m_generator && m_generator->done())
            m_generator = nullptr;
    }

    auto increment() -> void
    {
        assert(m_generator);
        m_generator->next();

        if (m_generator->done())
            m_generator = nullptr;
    }

    auto equal(const Iterator& o) const -> bool
    {
        return (m_empty && o.m_empty) || (!m_empty && !o.m_empty && !m_generator && !o.m_generator);
    }

    auto dereference() const -> const Element&
    {
        assert(m_generator);
        return m_generator->peek();
    }

public:
    Iterator() :
        m_generator{ nullptr },
        m_empty{ true }
    {}
};

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_GENERATOR_HPP_
