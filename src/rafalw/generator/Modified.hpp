#ifndef RAFALW_GENERATOR_MODIFIED_HPP_
#define RAFALW_GENERATOR_MODIFIED_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename G, typename M>
class Modified : public Base
{
public:
	template<typename G2, typename M2>
	Modified(G2&& g, M2&& m) :
		m_generator{ std::forward<G2>(g) },
		m_modifier{ std::forward<M2>(m) }
	{}

private:
	friend class BaseAccess;

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

template<typename G, typename M, require_instance<G> = nullptr>
auto modified(G&& g, M&& m) -> Modified<std::decay_t<G>, std::remove_reference_t<M>>
{
	return Modified<std::decay_t<G>, std::remove_reference_t<M>>{ std::forward<G>(g), std::forward<M>(m) };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_MODIFIED_HPP_
