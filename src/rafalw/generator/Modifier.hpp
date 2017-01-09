#ifndef RAFALW_GENERATOR_MODIFIER_HPP_
#define RAFALW_GENERATOR_MODIFIER_HPP_

#include <rafalw/generator/Base.hpp>
#include <rafalw/generator/Modified.hpp>
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

template<typename T>
auto upcast(const Modifier<T>& m) -> const T&
{
	return static_cast<const T&>(m);
}

template<typename T>
auto upcast(Modifier<T>& m) -> T&
{
	return static_cast<T&>(m);
}

template<typename M, typename G, typename require_instance<G> = {}>
auto done(const Modifier<M>& mod, const G& gen) -> decltype(ModifierAccess::done(upcast(mod), gen))
{
	return ModifierAccess::done(upcast(mod), gen);
}

template<typename M, typename G, typename require_instance<G> = {}>
auto peek(const Modifier<M>& mod, const G& gen) -> decltype(ModifierAccess::peek(upcast(mod), gen))
{
	return ModifierAccess::peek(upcast(mod), gen);
}

template<typename M, typename G, typename require_instance<G> = {}>
auto update(Modifier<M>& mod, G& gen) -> decltype(ModifierAccess::update(upcast(mod), gen))
{
	return ModifierAccess::update(upcast(mod), gen);
}

template<typename M, typename G, typename require_instance<G> = {}>
auto operator >>(G&& gen, Modifier<M> mod) -> decltype(modified(std::forward<G>(gen), std::move(mod)))
{
	return modified(std::forward<G>(gen), std::move(mod));
}

} // namespace generator
} // namespace rafalw

#endif /* RAFALW_GENERATOR_MODIFIER_HPP_ */
