#ifndef RAFALW_GENERATOR_TAKE_HPP_
#define RAFALW_GENERATOR_TAKE_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename GeneratorT>
class Take : private Base
{
public:
    using Generator = GeneratorT;

    template<typename G>
    Take(G&& generator, std::size_t n) :
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
        return done(m_generator) || m_count == m_max;
    }

    decltype(auto) generatorPeek() const
    {
        return peek(m_generator);
    }

    auto generatorUpdate() -> void
    {
        update(m_generator);
        m_count++;
    }

    auto generatorReset()
    {
        if constexpr (has_reset<Generator>)
        {
            m_count = 0;
            reset(m_generator);
            return RESET_OK;
        }
        else
        {
            return RESET_UNAVAILABLE;
        }
    }
};

template<typename GeneratorT>
auto take(GeneratorT&& generator, std::size_t count) -> Take<std::decay_t<GeneratorT>>
{
    return Take<std::decay_t<GeneratorT>>{ std::forward<GeneratorT>(generator), count };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_TAKE_HPP_
