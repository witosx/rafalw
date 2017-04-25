#ifndef RAFALW_GENERATOR_COMPOSITE_CYCLE_HPP_
#define RAFALW_GENERATOR_COMPOSITE_CYCLE_HPP_

#include <rafalw/generator/Wrapper.hpp>
#include <rafalw/generator/Wrapped.hpp>

inline namespace rafalw {
namespace generator {

class Cycle : public Wrapper
{
public:
    template<typename GeneratorT>
    auto wrapperDone(const GeneratorT& /*generator*/) const -> bool
    {
        return false;
    }

    template<typename GeneratorT>
    auto wrapperUpdate(GeneratorT& generator) -> void
    {
        update(generator);

        if (done(generator))
            reset(generator);
    }

    auto wrapperReset()
    {
        return RESET_OK;
    }
};

class CycleN : public Wrapper
{
public:
    CycleN(std::size_t n) :
        m_max{ n }
    {}

    template<typename GeneratorT>
    auto wrapperDone(const GeneratorT& /*generator*/) const -> bool
    {
        return m_count == m_max;
    }

    template<typename GeneratorT>
    auto wrapperUpdate(GeneratorT& generator) -> void
    {
        update(generator);

        if (done(generator))
        {
            reset(generator);
            m_count++;
        }
    }

    auto generatorReset()
    {
        m_count = 0;
        return RESET_OK;
    }

private:
    std::size_t m_max;
    std::size_t m_count = 0;
};

inline auto cycle() -> Cycle
{
    return Cycle{};
}

inline auto cycle(std::size_t n) -> CycleN
{
    return CycleN{ n };
}

template<typename GeneratorT, require_instance<GeneratorT> = nullptr>
auto cycle(GeneratorT&& generator) -> decltype(std::forward<GeneratorT>(generator) | cycle())
{
    return std::forward<GeneratorT>(generator) | cycle();
}

template<typename GeneratorT, require_instance<GeneratorT> = nullptr>
auto cycle(GeneratorT&& generator, std::size_t n) -> decltype(std::forward<GeneratorT>(generator) | cycle(n))
{
    return std::forward<GeneratorT>(generator) | cycle(n);
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_COMPOSITE_CYCLE_HPP_
