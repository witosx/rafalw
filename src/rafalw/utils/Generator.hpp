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
	static decltype(auto) done(const T& g)
	{
		return g.generatorDone();
	}

	template<typename T>
	static decltype(auto) peek(const T& g)
	{
		return g.generatorPeek();
	}

	template<typename T>
	static decltype(auto) update(T& g)
	{
		return g.generatorUpdate();
	}
};

template<typename DerivedT>
class Generator
{
public:
    auto done() const -> bool
	{
    	return GeneratorAccess::done(derived());
	}

    decltype(auto) peek() const
	{
        rafalw_utils_assert(!done());
    	return GeneratorAccess::peek(derived());
	}

    auto update() -> void
	{
        rafalw_utils_assert(!done());
    	return GeneratorAccess::update(derived());
	}

    auto next()
    {
        auto deferred_update = utils::scope_guard([this]{
            update();
        });

        return peek();
    }

    operator bool() const
    {
        return !done();
    }

    auto operator !() const -> bool
    {
        return done();
    }

    auto operator ()()
    {
        return next();
    }

private:
    using Derived = DerivedT;

    auto derived() -> Derived&
    {
        return static_cast<Derived&>(*this);
    }

    auto derived() const -> const Derived&
    {
        return static_cast<const Derived&>(*this);
    }
};

template<typename T>
struct GeneratorTraits;

template<typename G>
struct GeneratorTraits<Generator<G>>
{
	using Reference = decltype(std::declval<Generator<G>>().peek());
	using Value = std::remove_reference_t<Reference>;
};

template<typename T>
using GeneratorReference = typename GeneratorTraits<Generator<T>>::Reference;

template<typename T>
using GeneratorValue = typename GeneratorTraits<Generator<T>>::Value;



template<typename G>
class GeneratorIterator:
		public boost::iterator_facade<
					GeneratorIterator<G>,
					GeneratorValue<G>,
					boost::single_pass_traversal_tag,
					GeneratorReference<G>
					>
{
public:
	using Generator = Generator<G>;

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

    decltype(auto) dereference() const
    {
    	rafalw_utils_assert(m_generator);
        return m_generator->peek();
    }
};

template<typename G>
auto begin(Generator<G>& generator) -> GeneratorIterator<G>
{
    return GeneratorIterator<G>{ generator, false };
}

template<typename G>
auto end(Generator<G>& generator) -> GeneratorIterator<G>
{
    return GeneratorIterator<G>{ generator, true };
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_GENERATOR_HPP_
