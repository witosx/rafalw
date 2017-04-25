#ifndef RAFALW_GENERATOR_WRAPPED_HPP_
#define RAFALW_GENERATOR_WRAPPED_HPP_

#include <rafalw/generator/Base.hpp>
#include <rafalw/generator/simple/View.hpp>

inline namespace rafalw {
namespace generator {

template<typename GeneratorT, typename WrapperT>
class Wrapped : public Base
{
public:
    template<typename G, typename W>
    Wrapped(G&& generator, W&& wrapper) :
        m_generator{ std::forward<G>(generator) },
        m_wrapper{ std::forward<W>(wrapper) }
    {
        m_wrapper.wrapperInit(m_generator);
    }

private:
    friend class BaseAccess;

    using Generator = GeneratorT;
    using Wrapper = WrapperT;

    Generator m_generator;
    Wrapper m_wrapper;

    auto generatorDone() const -> bool
    {
        return m_wrapper.wrapperDone(m_generator);
    }

    auto generatorPeek() const -> decltype(m_wrapper.wrapperPeek(m_generator))
    {
        return m_wrapper.wrapperPeek(m_generator);
    }

    auto generatorUpdate() -> void
    {
        m_wrapper.wrapperUpdate(m_generator);
    }

    auto generatorReset()
    {
        using R1 = decltype(try_reset(m_generator));
        using R2 = decltype(m_wrapper.wrapperReset());

        static constexpr auto RESET_SUPPORTED = R1::value && R2::value;

        if (RESET_SUPPORTED)
        {
            try_reset(m_generator);
            m_wrapper.wrapperReset();
            m_wrapper.wrapperInit(m_generator);
        }

        return ResetTag<RESET_SUPPORTED>{};
    }
};

template<typename GeneratorT, typename WrapperT, require_instance<GeneratorT>* = nullptr>
auto wrap(GeneratorT&& genertor, WrapperT&& wrapper) -> Wrapped<std::decay_t<GeneratorT>, std::decay_t<WrapperT>>
{
    return Wrapped<std::decay_t<GeneratorT>, std::decay_t<WrapperT>>{ std::forward<GeneratorT>(genertor), std::forward<WrapperT>(wrapper) };
}

template<typename GeneratorT, typename WrapperT, require_instance<GeneratorT>* = nullptr>
auto operator |(GeneratorT&& genertor, WrapperT&& wrapper) -> decltype(wrap(std::forward<GeneratorT>(genertor), std::forward<WrapperT>(wrapper)))
{
    return wrap(std::forward<GeneratorT>(genertor), std::forward<WrapperT>(wrapper));
}

template<typename IterableT, typename WrapperT, std::enable_if_t<!is_instance<IterableT>>* = nullptr>
auto operator |(IterableT&& iterable, WrapperT&& wrapper) -> decltype(wrap(view(std::forward<IterableT>(iterable)), std::forward<WrapperT>(wrapper)))
{
    return wrap(view(std::forward<IterableT>(iterable)), std::forward<WrapperT>(wrapper));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_WRAPPED_HPP_
