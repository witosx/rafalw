#ifndef XTRAS_GENERATOR_MODIFIERFILTERING_HPP_
#define XTRAS_GENERATOR_MODIFIERFILTERING_HPP_

#include <xtras/generator/Wrapper.hpp>
#include <xtras/generator/Wrapped.hpp>

namespace xtras {
namespace generator {

template<typename FunctionT>
class Filter : public Wrapper
{
public:
	template<typename F, std::enable_if_t<!std::is_same<std::decay_t<F>, Filter<FunctionT>>::value>* = nullptr>
	Filter(F&& f) :
		m_function{ std::forward<F>(f) }
	{}

    template<typename G>
    auto wrapperInit(G& g) -> void
    {
        skip(g);
    }

	template<typename G>
    auto wrapperUpdate(G& g) -> void
    {
	    update(g);
	    skip(g);
    }

    auto wrapperReset()
    {
        return RESET_TAG<true>;
    }

private:
	using Function = FunctionT;

	Function m_function;

	template<typename G>
	auto skip(G& g) -> void
    {
        while (!done(g) && !m_function(peek(g)))
            update(g);
    }
};

template<typename F>
auto filter(F&& f) -> Filter<std::decay_t<F>>
{
    return Filter<std::decay_t<F>>{ std::forward<F>(f) };
}

template<typename G, typename F, require_instance<G> = nullptr>
auto filter(G&& gen, F&& f) -> decltype(std::forward<G>(gen) | filter(f))
{
    return std::forward<G>(gen) | filter(f);
}

} // namespace generator
} // namespace xtras

#endif // XTRAS_GENERATOR_MODIFIERFILTERING_HPP_
