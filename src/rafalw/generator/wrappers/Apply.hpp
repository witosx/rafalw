#ifndef RAFALW_GENERATOR_WRAPPERS_APPLY_HPP_
#define RAFALW_GENERATOR_WRAPPERS_APPLY_HPP_

#include <rafalw/generator/Wrapper.hpp>
#include <rafalw/generator/Wrapped.hpp>

inline namespace rafalw {
namespace generator {

template<typename FunctionT>
class Apply : public Wrapper
{
public:
    template<typename F, std::enable_if_t<!std::is_same<std::decay_t<F>, Apply<FunctionT>>::value>* = nullptr>
    Apply(F&& f) :
        m_function{ std::forward<F>(f) }
    {}

    template<typename G>
    decltype(auto) wrapperPeek(const G& g) const
    {
        return m_function(peek(g));
    }

    auto wrapperReset()
    {
        return RESET_TAG<true>;
    }

private:
    using Function = FunctionT;

    Function m_function;
};

template<typename F>
auto apply(F&& f) -> Apply<std::decay_t<F>>
{
    return Apply<std::decay_t<F>>{ std::forward<F>(f) };
}

template<typename G, typename F, require_instance<G> = nullptr>
auto apply(G&& gen, F&& f) -> decltype(std::forward<G>(gen) | apply(f))
{
    return std::forward<G>(gen) | apply(f);
}

} // namespace generator
} // namespace rafalw
#endif // RAFALW_GENERATOR_WRAPPERS_APPLY_HPP_
