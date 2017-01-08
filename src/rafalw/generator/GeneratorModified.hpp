#ifndef RAFALW_GENERATOR_GENERATORMODIFIED_HPP_
#define RAFALW_GENERATOR_GENERATORMODIFIED_HPP_

#include <rafalw/generator/Generator.hpp>

inline namespace rafalw {
namespace generator {

template<typename G, typename M>
class GeneratorModified : public Generator<GeneratorModified<G, M>>
{
public:
	template<typename G2, typename M2>
	GeneratorModified(G2&& g, M2&& m) :
		m_generator{ std::forward<G2>(g) },
		m_modifier{ std::forward<M2>(m) }
	{}

private:
	friend class AccessProxy;

	using BaseGenerator = G;
	using BaseModifier = M;

	BaseGenerator m_generator;
	BaseModifier m_modifier;

    auto generatorDone() const -> decltype(done(m_modifier, m_generator))
    {
    	return done(m_modifier, m_generator);
    }

    auto generatorPeek() const -> decltype(peek(m_modifier, m_generator))
    {
    	return peek(m_modifier, m_generator);
    }

    auto generatorUpdate() -> decltype(update(m_modifier, m_generator))
    {
    	return update(m_modifier, m_generator);
    }
};

template<typename G, typename M, typename = require_instance<G>>
auto generator_modified(G&& g, M&& m) -> GeneratorModified<G&&, std::remove_reference_t<M>>
{
	return GeneratorModified<G&&, std::remove_reference_t<M>>{ std::forward<G>(g), std::forward<M>(m) };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_GENERATORMODIFIED_HPP_
