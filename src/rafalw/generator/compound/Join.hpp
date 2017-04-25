#ifndef RAFALW_GENERATOR_JOIN_HPP_
#define RAFALW_GENERATOR_JOIN_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
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
    {}

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
        if (m_index == 0)
            return peek(m_generator1);
        else
            return peek(m_generator2);
    }

    auto generatorUpdate() -> void
    {
        if (m_index == 0)
            updateGenerator(m_generator1);
        else
            updateGenerator(m_generator2);
    }

    auto generatorReset()
    {
        static constexpr auto RES1 = has_reset<Generator1>;
        static constexpr auto RES2 = has_reset<Generator2>;
        static constexpr auto RES = RES1 && RES2;

        if (RES)
        {
            try_reset(m_generator1);
            try_reset(m_generator2);
            m_index = 0;
        }

        return ResetTag<RES>{};
    }

    template<typename G>
    auto updateGenerator(G& g) -> void
    {
        update(g);
        if (done(g))
            m_index++;
    }
};

template<typename GeneratorT1, typename GeneratorT2>
auto join(GeneratorT1&& g1, GeneratorT2&& g2) -> Join<std::decay_t<GeneratorT1>, std::decay_t<GeneratorT2>>
{
    return Join<std::decay_t<GeneratorT1>, std::decay_t<GeneratorT2>>{ std::forward<GeneratorT1>(g1), std::forward<GeneratorT2>(g2) };
}

template<typename GeneratorT1, typename GeneratorT2, require_instance<GeneratorT1> = nullptr, require_instance<GeneratorT2> = nullptr>
auto operator +(GeneratorT1&& g1, GeneratorT2&& g2) -> decltype(join(std::forward<GeneratorT1>(g1), std::forward<GeneratorT2>(g2)))
{
    return join(std::forward<GeneratorT1>(g1), std::forward<GeneratorT2>(g2));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_JOIN_HPP_
