#ifndef RAFALW_GENERATOR_FILTER_HPP_
#define RAFALW_GENERATOR_FILTER_HPP_

#include <rafalw/generator/Modifier.hpp>

inline namespace rafalw {
namespace generator {

template<typename F>
class Filter : public Modifier<Filter<F>>
{
public:
	template<typename F2>
	Filter(F2&& f) :
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

			if (!gen || m_function(peek(gen)))
				break;
		}

	}
};

template<typename F>
auto filter(F&& f) -> Filter<std::remove_reference_t<F>>
{
	return Filter<std::remove_reference_t<F>>{ std::forward<F>(f) };
}

template<typename D, typename F>
auto operator |(Generator<D>& gen, F&& f) -> decltype(gen >> filter(std::forward<F>(f)))
{
	return gen >> filter(std::forward<F>(f));
}

template<typename D, typename F>
auto operator |(Generator<D>&& gen, F&& f) -> decltype(std::move(gen) >> filter(std::forward<F>(f)))
{
	return std::move(gen) >> filter(std::forward<F>(f));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_FILTER_HPP_
