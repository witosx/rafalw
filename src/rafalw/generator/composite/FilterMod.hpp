#ifndef RAFALW_GENERATOR_MODIFIERFILTERING_HPP_
#define RAFALW_GENERATOR_MODIFIERFILTERING_HPP_

#include <rafalw/generator/ModifierBase.hpp>

inline namespace rafalw {
namespace generator {

template<typename F>
class FilterMod : public ModifierBase<FilterMod<F>>
{
public:
	template<typename F2>
	FilterMod(F2&& f) :
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
auto filter_mod(F&& f) -> FilterMod<std::remove_reference_t<F>>
{
	return FilterMod<std::remove_reference_t<F>>{ std::forward<F>(f) };
}

template<typename G, typename F, require_instance<G> = nullptr>
auto filter(G&& gen, F&& f) -> decltype(modified(std::forward<G>(gen), filter_mod(std::forward<F>(f))))
{
    return modified(std::forward<G>(gen), filter_mod(std::forward<F>(f)));
}

template<typename G, typename F, require_instance<G> = nullptr>
auto operator |(G&& gen, F&& f) -> decltype(filter(std::forward<G>(gen), std::forward<F>(f)))
{
	return filter(std::forward<G>(gen), std::forward<F>(f));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_MODIFIERFILTERING_HPP_
