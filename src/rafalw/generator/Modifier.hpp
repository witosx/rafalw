#ifndef RAFALW_GENERATOR_MODIFIER_HPP_
#define RAFALW_GENERATOR_MODIFIER_HPP_

#include <rafalw/generator/GeneratorModified.hpp>
#include <rafalw/generator/Generator.hpp>
#include <rafalw/generator/ModifierAccess.hpp>

inline namespace rafalw {
namespace generator {

template<typename DerivedT>
class Modifier
{
protected:
	template<typename G>
	auto generatorDone(const G& gen) const -> decltype(done(gen))
	{
		return done(gen);
	}

	template<typename G>
	auto generatorPeek(const G& gen) const -> decltype(peek(gen))
	{
		return peek(gen);
	}

	template<typename G>
	auto generatorUpdate(G& gen) -> decltype(update(gen))
	{
		return update(gen);
	}
};

template<typename M, typename G>
auto done(const Modifier<M>& mod, const G& gen) -> decltype(ModifierAccess::done(static_cast<const M&>(mod), gen))
{
	return ModifierAccess::done(static_cast<const M&>(mod), gen);
}

template<typename M, typename G>
auto peek(const Modifier<M>& mod, const G& gen) -> decltype(ModifierAccess::peek(static_cast<const M&>(mod), gen))
{
	return ModifierAccess::peek(static_cast<const M&>(mod), gen);
}

template<typename M, typename G>
auto update(Modifier<M>& mod, G& gen) -> decltype(ModifierAccess::update(static_cast<M&>(mod), gen))
{
	return ModifierAccess::update(static_cast<M&>(mod), gen);
}

template<typename D, typename M>
auto operator >>(Generator<D>& gen, Modifier<M> mod) -> GeneratorModified<Generator<D>&, Modifier<M>>
{
	return GeneratorModified<Generator<D>&, Modifier<M>>{ gen, mod };
}

template<typename D, typename M>
auto operator >>(Generator<D>&& gen, Modifier<M> mod) -> GeneratorModified<Generator<D>&&, Modifier<M>>
{
	return GeneratorModified<Generator<D>&&, Modifier<M>>{ std::move(gen), mod };
}

} // namespace generator
} // namespace rafalw

#endif /* RAFALW_GENERATOR_MODIFIER_HPP_ */
