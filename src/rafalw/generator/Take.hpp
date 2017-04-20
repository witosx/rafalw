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
    Take(G&& generator, std::size_t count) :
        m_generator{ std::forward<G>(generator) },
        m_count{ count }
    {}

private:
    friend class BaseAccess;

    Generator m_generator;
    std::size_t m_count;

    auto generatorDone() const -> bool
    {
        return done(m_generator) || m_count == 0;
    }

    decltype(auto) generatorPeek() const
    {
        return peek(m_generator);
    }

    auto generatorUpdate() -> void
    {
        update(m_generator);
        --m_count;
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
