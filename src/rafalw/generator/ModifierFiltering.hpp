#ifndef RAFALW_GENERATOR_MODIFIERFILTERING_HPP_
#define RAFALW_GENERATOR_MODIFIERFILTERING_HPP_

#include <rafalw/generator/ModifierBase.hpp>

inline namespace rafalw {
namespace generator {

template<typename F>
class ModifierFiltering : public ModifierBase<ModifierFiltering<F>>
{
public:
	template<typename F2>
	ModifierFiltering(F2&& f) :
		m_function{ std::forward<F2>(f) }
	{}

private:
	friend class ModifierAccess;

	using Function = F;

	Function m_function;

	template<typename G>
	auto generatorUpdate(G& gen) const -> void
	{
		while (true)
		{
			update(gen);

			if (done(gen) || m_function(peek(gen)))
				break;
		}
	}
};

template<typename F>
auto modifier_filtering(F&& f) -> ModifierFiltering<std::remove_reference_t<F>>
{
	return ModifierFiltering<std::remove_reference_t<F>>{ std::forward<F>(f) };
}

template<typename G, typename F, require_instance<G> = nullptr>
auto filter(G&& gen, F&& f) -> decltype(modified(std::forward<G>(gen), modifier_filtering(std::forward<F>(f))))
{
    return modified(std::forward<G>(gen), modifier_filtering(std::forward<F>(f)));
}

template<typename G, typename F, require_instance<G> = nullptr>
auto operator |(G&& gen, F&& f) -> decltype(filter(std::forward<G>(gen), std::forward<F>(f)))
{
	return filter(std::forward<G>(gen), std::forward<F>(f));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_MODIFIERFILTERING_HPP_
