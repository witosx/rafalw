#ifndef RAFALW_UTILS_BUFFERINGGENERATOR_HPP_
#define RAFALW_UTILS_BUFFERINGGENERATOR_HPP_

#include <rafalw/utils/BasicGenerator.hpp>
#include <rafalw/utils/ScopeGuard.hpp>
#include <rafalw/utils/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>

inline namespace rafalw {
namespace utils {

template<typename _Derived, typename _Element>
class BufferingGenerator : public BasicGenerator<_Derived, _Element>
{
public:
    using Element = _Element;

    auto next() -> Element
    {
        rafalw_utils_assert(!done());

        auto deferred_update = utils::scope_guard([this](){
            m_element = derived().fetch();
        });

        return *std::move(m_element);
    }

    auto peek() const -> const Element&
    {
        rafalw_utils_assert(!done());
        return *m_element;
    }

    auto done() const -> bool
    {
        return !static_cast<bool>(m_element);
    }

    operator bool() const
    {
        return !done();
    }

    auto operator !() const -> bool
    {
        return !static_cast<bool>(*this);
    }

private:
    using Derived = _Derived;

    boost::optional<Element> m_element;
};

template<typename D, typename E>
class BufferingGeneratorIterator :
    public boost::iterator_facade<BufferingGeneratorIterator<D, E>, const E, boost::single_pass_traversal_tag>
{
public:
    using Generator = BufferingGenerator<D, E>;
    using Element = E;

    BufferingGeneratorIterator() = default;

    BufferingGeneratorIterator(Generator& generator, bool end) :
        m_generator{ generator },
        m_end{ end }
    {}

private:
    friend class boost::iterator_core_access;

    Generator* m_generator = nullptr;
    bool m_end = false;

    auto increment() -> void
    {
        assert(m_generator);
        m_generator->next();
        m_end = m_generator->done();
    }

    auto equal(const Iterator& o) const -> bool
    {
        return m_end == o.m_end && m_generator == o.m_generator;
    }

    auto dereference() const -> const Element&
    {
        assert(m_generator);
        return m_generator->peek();
    }
};

template<typename D, typename E>
auto begin(BufferingGenerator<D, E>& gen) -> BufferingGeneratorIterator<D, E>
{
    return BufferingGeneratorIterator<D, E>{ gen, false };
}

template<typename D, typename E>
auto end(BufferingGenerator<D, E>& gen) -> BufferingGeneratorIterator<D, E>
{
    return BufferingGeneratorIterator<D, E>{ gen, true };
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_BUFFERINGGENERATOR_HPP_
