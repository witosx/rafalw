#ifndef RAFALW_GENERATOR_WRAPPERS_TRANSFORM_HPP_
#define RAFALW_GENERATOR_WRAPPERS_TRANSFORM_HPP_

#include <rafalw/generator/Wrapper.hpp>
#include <rafalw/generator/Wrapped.hpp>

inline namespace rafalw {
namespace generator {

template<typename FunctionT>
class Transform : public Wrapper
{
public:
    template<typename F>
    Transform(F&& f) :
        m_function{ std::forward<F>(f) }
    {}

    template<typename G>
    decltype(auto) wrapperPeek(const G& g) const
    {
        return m_function(peek(g));
    }

    auto wrapperReset()
    {
        return RESET_OK;
    }

private:
    friend class BaseAccess;

    using Function = FunctionT;

    Function m_function;
};

template<typename F>
auto transform(F&& f) -> Transform<std::decay_t<F>>
{
    return Transform<std::decay_t<F>>{ std::forward<F>(f) };
}

template<typename G, typename F, require_instance<G> = nullptr>
auto transform(G&& gen, F&& f) -> decltype(std::forward<G>(gen) | transform(f))
{
    return std::forward<G>(gen) | transform(f);
}

} // namespace generator
} // namespace rafalw
#endif // RAFALW_GENERATOR_WRAPPERS_TRANSFORM_HPP_
