#ifndef RAFALW_UTILS_GENERATOR_HPP_
#define RAFALW_UTILS_GENERATOR_HPP_

#include <rafalw/utils/ScopeGuard.hpp>
#include <rafalw/utils/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>

inline namespace rafalw {
namespace utils {

class GeneratorAccess
{
public:
	template<typename T>
	static auto done(const T& g) -> decltype(g.generatorDone())
	{
		return g.generatorDone();
	}

	template<typename T>
	static auto peek(const T& g) -> decltype(g.generatorPeek())
	{
		return g.generatorPeek();
	}

    template<typename T>
    static auto take(T& g) -> decltype(g.generatorTake())
    {
        return g.generatorTake();
    }

	template<typename T>
	static auto update(T& g) -> decltype(g.generatorUpdate())
	{
		return g.generatorUpdate();
	}
};

template<typename _Derived, typename _Element>
class Generator
{
public:
    using Element = _Element;

    auto done() const -> bool
	{
    	return GeneratorAccess::done(derived());
	}

    auto peek() const -> const Element&
	{
        rafalw_utils_assert(!done());
    	return GeneratorAccess::peek(derived());
	}

    auto update() -> void
	{
        rafalw_utils_assert(!done());
    	return GeneratorAccess::update(derived());
	}

    auto next() -> Element
    {
        auto deferred_update = utils::scope_guard([this]{
            update();
        });

        return take();
    }

    operator bool() const
    {
        return !done();
    }

    auto operator !() const -> bool
    {
        return done();
    }

    auto operator ()() -> Element
    {
        return next();
    }

protected:
    auto generatorTake() -> Element
    {
        return peek();
    }

private:
    using Derived = _Derived;

    auto derived() -> Derived&
    {
        return static_cast<Derived&>(*this);
    }

    auto derived() const -> const Derived&
    {
        return static_cast<const Derived&>(*this);
    }

    auto take() -> Element
    {
        return GeneratorAccess::take(derived());
    }
};

template<typename G, typename E>
class GeneratorIterator: public boost::iterator_facade<GeneratorIterator<G, E>, const E, boost::single_pass_traversal_tag>
{
public:
	using Generator = Generator<G, E>;
	using Element = typename Generator::Element;

	GeneratorIterator() = default;

    GeneratorIterator(Generator& generator, bool end) :
        m_generator{ &generator },
		m_end{ end || generator.done() }
    {}

private:
    friend class boost::iterator_core_access;

    Generator* m_generator = nullptr;
    bool m_end = false;

    auto increment() -> void
    {
        rafalw_utils_assert(m_generator);
        rafalw_utils_assert(!m_end);

        m_generator->update();
        m_end = m_generator->done();
    }

    auto equal(const GeneratorIterator& o) const -> bool
    {
        return m_end == o.m_end && m_generator == o.m_generator;
    }

    auto dereference() const -> const Element&
    {
    	rafalw_utils_assert(m_generator);
        return m_generator->peek();
    }
};

template<typename G, typename E>
auto begin(Generator<G, E>& generator) -> GeneratorIterator<G, E>
{
    return GeneratorIterator<G, E>{ generator, false };
}

template<typename G, typename E>
auto end(Generator<G, E>& generator) -> GeneratorIterator<G, E>
{
    return GeneratorIterator<G, E>{ generator, true };
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_GENERATOR_HPP_
