#ifndef RAFALW_UTILS_BASICGENERATOR_HPP_
#define RAFALW_UTILS_BASICGENERATOR_HPP_

#include <rafalw/utils/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>

inline namespace rafalw {
namespace utils {

template<typename _Derived, typename _Element>
class BasicGenerator
{
protected:
    using Derived = _Derived;

    auto derived() -> Derived&
    {
        return static_cast<Derived&>(*this);
    }

public:
    using Element = _Element;

    auto next() -> boost::optional<Element>
    {
        return derived().fetch();
    }

    auto operator ()() -> decltype(derived().next())
    {
        return derived().next();
    }
};


template<typename D, typename E>
class BasicGeneratorIterator :
    public boost::iterator_facade<BasicGeneratorIterator<D, E>, const E, boost::single_pass_traversal_tag>
{
public:
    using Generator = BasicGenerator<D, E>;

    struct EndTag {};

    BasicGeneratorIterator() = default;

    explicit BasicGeneratorIterator(Generator& generator) :
        m_generator{ &generator }
    {
        increment();
    }

    BasicGeneratorIterator(Generator& generator, EndTag) :
        m_generator{ &generator }
    {}

private:
    friend class boost::iterator_core_access;

    using Element = typename Generator::Element;

    Generator* m_generator = nullptr;
    boost::optional<Element> m_element;

    auto increment() -> void
    {
        rafalw_utils_assert(m_generator);
        m_element = m_generator->next();
    }

    auto equal(const Iterator& o) const -> bool
    {
        return (m_empty && o.m_empty) || (!m_empty && !o.m_empty && !m_generator && !o.m_generator);
    }

    auto dereference() const -> const Element&
    {
        rafalw_utils_assert(m_element);
        return *m_element;
    }
};

template<typename D, typename E>
auto begin(BasicGenerator<D, E>& gen) -> BasicGeneratorIterator<D, E>
{
    return BasicGeneratorIterator<D, E>{ gen };
}

template<typename D, typename E>
auto end(BasicGenerator<D, E>& gen) -> BasicGeneratorIterator<D, E>
{
    return BasicGeneratorIterator<D, E>{ gen, typename BasicGeneratorIterator<D, E>::EndTag{} };
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_BASICGENERATOR_HPP_
