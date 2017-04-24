#ifndef RAFALW_GENERATOR_MODIFIERTRANSFORMING_HPP_
#define RAFALW_GENERATOR_MODIFIERTRANSFORMING_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename GeneratorT, typename FunctionT>
class Transform : private Base
{
public:
	template<typename G, typename F>
	Transform(G&& g, F&& f) :
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
        return m_function(peek(m_generator));
    }

    auto generatorUpdate() -> void
    {
        update(m_generator);
    }

    auto generatorReset()
    {
        return try_reset(m_generator);
    }
};

template<typename G, typename F, require_instance<G> = nullptr>
auto transform(G&& gen, F&& f) -> Transform<std::decay_t<G>, std::decay_t<F>>
{
    return Transform<std::decay_t<G>, std::decay_t<F>>{ std::forward<G>(gen), std::forward<F>(f) };
}

template<typename G, typename F, require_instance<G> = nullptr>
auto operator %(G&& gen, F&& f) -> decltype(transform(std::forward<G>(gen), std::forward<F>(f)))
{
	return transform(std::forward<G>(gen), std::forward<F>(f));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_MODIFIERTRANSFORMING_HPP_
