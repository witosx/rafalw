#ifndef RAFALW_GENERATOR_MODIFIERTRANSFORMING_HPP_
#define RAFALW_GENERATOR_MODIFIERTRANSFORMING_HPP_

#include <rafalw/generator/ModifierBase.hpp>

inline namespace rafalw {
namespace generator {

template<typename F>
class ModifierTransforming : public ModifierBase<ModifierTransforming<F>>
{
public:
	template<typename F2>
	ModifierTransforming(F2&& f) :
		m_function{ std::forward<F2>(f) }
	{}

private:
	friend class ModifierAccess;

	using Function = F;

	Function m_function;

	template<typename G>
	auto generatorPeek(const G& gen) const -> decltype(m_function(peek(gen)))
	{
		return m_function(peek(gen));
	}
};

template<typename F>
auto modifier_transforming(F&& f) -> ModifierTransforming<std::remove_reference_t<F>>
{
	return ModifierTransforming<std::remove_reference_t<F>>{ std::forward<F>(f) };
}

template<typename G, typename F, require_instance<G> = nullptr>
auto transform(G&& gen, F&& f) -> decltype(modified(std::forward<G>(gen), modifier_transforming(std::forward<F>(f))))
{
    return modified(std::forward<G>(gen), modifier_transforming(std::forward<F>(f)));
}

template<typename G, typename F, require_instance<G> = nullptr>
auto operator %(G&& gen, F&& f) -> decltype(transform(std::forward<G>(gen), std::forward<F>(f)))
{
	return transform(std::forward<G>(gen), std::forward<F>(f));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_MODIFIERTRANSFORMING_HPP_
