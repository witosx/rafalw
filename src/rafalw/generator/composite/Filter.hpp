#ifndef RAFALW_GENERATOR_MODIFIERFILTERING_HPP_
#define RAFALW_GENERATOR_MODIFIERFILTERING_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename GeneratorT, typename FunctionT>
class Filter : private Base
{
public:
	template<typename G, typename F>
	Filter(G&& g, F&& f) :
	    m_generator{ std::forward<G>(g) },
		m_function{ std::forward<F>(f) }
	{}

private:
	friend class BaseAccess;

	using Generator = GeneratorT;
	using Function = FunctionT;

	Generator m_generator;
	Function m_function;

    auto generatorDone() const -> bool
    {
        return done(m_generator);
    }

    decltype(auto) generatorPeek() const
    {
        return peek(m_generator);
    }

    auto generatorUpdate() -> void
    {
        while (true)
        {
            update(m_generator);
            if (done(m_generator) || m_function(peek(m_generator)))
                break;
        }
    }

    auto generatorReset()
    {
        return try_reset(m_generator);
    }
};

template<typename G, typename F, require_instance<G> = nullptr>
auto filter(G&& gen, F&& f) -> Filter<std::decay_t<G>, std::decay_t<F>>
{
    return Filter<std::decay_t<G>, std::decay_t<F>>{ std::forward<G>(gen), std::forward<F>(f) };
}

template<typename G, typename F, require_instance<G> = nullptr>
auto operator |(G&& gen, F&& f) -> decltype(filter(std::forward<G>(gen), std::forward<F>(f)))
{
	return filter(std::forward<G>(gen), std::forward<F>(f));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_MODIFIERFILTERING_HPP_
