#ifndef RAFALW_GENERATOR_COMPOSITE_CYCLE_HPP_
#define RAFALW_GENERATOR_COMPOSITE_CYCLE_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename GeneratorT>
class Cycle : private Base
{
public:
    using Generator = GeneratorT;

    template<typename G>
    Cycle(ConstructTag, G&& generator) :
        m_generator{ std::forward<G>(generator) }
    {}

private:
    friend class BaseAccess;

    Generator m_generator;

    auto generatorDone() const -> bool
    {
        return false;
    }

    decltype(auto) generatorPeek() const
    {
        return peek(m_generator);
    }

    auto generatorUpdate() -> void
    {
        update(m_generator);

        if (done(m_generator))
            reset(m_generator);
    }

    auto generatorReset()
    {
        reset(m_generator);
        return RESET_OK;
    }
};

template<typename GeneratorT>
class CycleN : private Base
{
public:
    using Generator = GeneratorT;

    template<typename G>
    CycleN(ConstructTag, G&& generator, std::size_t n) :
        m_generator{ std::forward<G>(generator) },
        m_max{ n }
    {}

private:
    friend class BaseAccess;

    Generator m_generator;
    std::size_t m_max;

    std::size_t m_count = 0;

    auto generatorDone() const -> bool
    {
        return m_count == m_max;
    }

    decltype(auto) generatorPeek() const
    {
        return peek(m_generator);
    }

    auto generatorUpdate() -> void
    {
        update(m_generator);

        if (done(m_generator))
        {
            reset(m_generator);
            m_count++;
        }
    }

    auto generatorReset()
    {
        reset(m_generator);
        m_count = 0;
        return RESET_OK;
    }
};

template<typename GeneratorT>
auto cycle(GeneratorT&& generator) -> Cycle<std::decay_t<GeneratorT>>
{
    return Cycle<std::decay_t<GeneratorT>>{ ConstructTag{}, std::forward<GeneratorT>(generator) };
}

template<typename GeneratorT>
auto cycle(GeneratorT&& generator, std::size_t n) -> CycleN<std::decay_t<GeneratorT>>
{
    return CycleN<std::decay_t<GeneratorT>>{ ConstructTag{}, std::forward<GeneratorT>(generator), n };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_COMPOSITE_CYCLE_HPP_
