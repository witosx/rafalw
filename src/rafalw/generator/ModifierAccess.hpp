#ifndef RAFALW_GENERATOR_MODIFIERACCESS_HPP_
#define RAFALW_GENERATOR_MODIFIERACCESS_HPP_

inline namespace rafalw {
namespace generator {

class ModifierAccess
{
public:
	template<typename M, typename G>
	static auto done(const M& mod, const G& gen) -> decltype(mod.generatorDone(gen))
	{
		return mod.generatorDone(gen);
	}

	template<typename M, typename G>
	static auto peek(const M& mod, const G& gen) -> decltype(mod.generatorPeek(gen))
	{
		return mod.generatorPeek(gen);
	}

	template<typename M, typename G>
	static auto update(M& mod, G& gen) -> decltype(mod.generatorUpdate(gen))
	{
		return mod.generatorUpdate(gen);
	}
};

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_MODIFIERACCESS_HPP_
