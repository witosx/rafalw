#ifndef XTRAS_GENERATOR_JOIN_HPP_
#define XTRAS_GENERATOR_JOIN_HPP_

#include <xtras/generator/Base.hpp>
#include <xtras/generator/simple/View.hpp>

namespace xtras {
namespace generator {

namespace detail {

    template<typename T>
    struct JoinValueTypeBase
    {
        using Type = T;
    };

    template<typename T1, typename T2>
    struct JoinValueType;

    template<typename T> struct JoinValueType<T, T>         : public JoinValueTypeBase<T> {};

    template<typename T> struct JoinValueType<T, T&>        : public JoinValueTypeBase<T> {};
    template<typename T> struct JoinValueType<T, const T&>  : public JoinValueTypeBase<T> {};
    template<typename T> struct JoinValueType<T&, T>        : public JoinValueTypeBase<T> {};
    template<typename T> struct JoinValueType<const T&, T>  : public JoinValueTypeBase<T> {};

    template<typename T> struct JoinValueType<const T&, T&> : public JoinValueTypeBase<const T&> {};
    template<typename T> struct JoinValueType<T&, const T&> : public JoinValueTypeBase<const T&> {};

    template<typename G1, typename G2>
    using JoinValue = typename JoinValueType<decltype(peek(std::declval<G1>())), decltype(peek(std::declval<G2>()))>::Type;

} // namespace detail

template<typename GeneratorT1, typename GeneratorT2, typename ValueT = detail::JoinValue<GeneratorT1, GeneratorT2>>
class Join : private Base
{
public:
    template<typename G1, typename G2>
    Join(G1&& g1, G2&& g2) :
        m_generator1{ std::forward<G1>(g1) },
        m_generator2{ std::forward<G2>(g2) }
    {
        currentInit();
    }

private:
    friend class BaseAccess;

    using Generator1 = GeneratorT1;
    using Generator2 = GeneratorT2;

    using Value = ValueT;

    Generator1 m_generator1;
    Generator2 m_generator2;

    int m_index = 0;

    auto generatorDone() const -> bool
    {
        return m_index == 2;
    }

    auto generatorPeek() const -> Value
    {
        return currentApply([](const auto& g) -> Value {
            return peek(g);
        });
    }

    auto generatorUpdate() -> void
    {
        currentApply([this](auto& g) {
            update(g);
            currentInit();
        });
    }

    auto generatorReset()
    {
        static constexpr auto OK1 = reset_ok<Generator1>;
        static constexpr auto OK2 = reset_ok<Generator2>;
        static constexpr auto OK = OK1 && OK2;

        if (OK)
        {
            try_reset(m_generator1);
            try_reset(m_generator2);
            m_index = 0;
            currentInit();
        }

        return RESET_TAG<OK>;
    }

    template<typename F>
    decltype(auto) currentApply(F&& f)
    {
        if (m_index == 0)
            return f(m_generator1);
        else
            return f(m_generator2);
    }

    template<typename F>
    decltype(auto) currentApply(F&& f) const
    {
        if (m_index == 0)
            return f(m_generator1);
        else
            return f(m_generator2);
    }

    auto currentDone() const -> bool
    {
        return currentApply([](const auto& g) {
            return done(g);
        });
    }

    auto currentInit() -> void
    {
        while (currentDone() && !generatorDone())
            m_index++;
    }
};

template<typename GeneratorT1, typename GeneratorT2>
auto join(GeneratorT1&& g1, GeneratorT2&& g2) -> Join<std::decay_t<GeneratorT1>, std::decay_t<GeneratorT2>>
{
    return Join<std::decay_t<GeneratorT1>, std::decay_t<GeneratorT2>>{ std::forward<GeneratorT1>(g1), std::forward<GeneratorT2>(g2) };
}

template<typename GeneratorT1, typename GeneratorT2, std::enable_if_t<is_generator<GeneratorT1> && is_generator<GeneratorT2>>* = nullptr>
auto operator |(GeneratorT1&& g1, GeneratorT2&& g2) -> decltype(join(std::forward<GeneratorT1>(g1), std::forward<GeneratorT2>(g2)))
{
    return join(std::forward<GeneratorT1>(g1), std::forward<GeneratorT2>(g2));
}

template<typename IterableT, typename GeneratorT, std::enable_if_t<!is_generator<IterableT> && !is_wrapper<IterableT> && is_generator<GeneratorT>>* = nullptr>
auto operator |(IterableT&& iterable, GeneratorT&& gen) -> decltype(generator::view(std::forward<IterableT>(iterable)) | std::forward<GeneratorT>(gen))
{
    return generator::view(std::forward<IterableT>(iterable)) | std::forward<GeneratorT>(gen);
}

template<typename IterableT, typename GeneratorT, std::enable_if_t<!is_generator<IterableT> && !is_wrapper<IterableT> && is_generator<GeneratorT>>* = nullptr>
auto operator |(GeneratorT&& gen, IterableT&& iterable) -> decltype(std::forward<GeneratorT>(gen) | generator::view(std::forward<IterableT>(iterable)))
{
    return std::forward<GeneratorT>(gen) | generator::view(std::forward<IterableT>(iterable));
}

} // namespace generator
} // namespace xtras

#endif // XTRAS_GENERATOR_JOIN_HPP_
