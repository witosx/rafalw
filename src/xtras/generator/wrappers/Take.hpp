#ifndef XTRAS_GENERATOR_TAKE_HPP_
#define XTRAS_GENERATOR_TAKE_HPP_

#include <xtras/generator/Wrapper.hpp>
#include <xtras/generator/Wrapped.hpp>

namespace xtras {
namespace generator {

class Take : public Wrapper
{
public:
    explicit Take(std::size_t n) :
        m_max{ n }
    {}

    template<typename G>
    auto wrapperDone(const G& g) const -> bool
    {
        return done(g) || m_count == m_max;
    }

    template<typename G>
    auto wrapperUpdate(G& g) -> void
    {
        update(g);
        m_count++;
    }

    auto wrapperReset()
    {
        m_count = 0;
        return RESET_TAG<true>;
    }

private:
    std::size_t m_max;
    std::size_t m_count = 0;
};

inline auto take(std::size_t count) -> Take
{
    return Take{ count };
}

template<typename GeneratorT>
inline auto take(GeneratorT&& generator, std::size_t count) -> decltype(std::forward<GeneratorT>(generator) | take(count))
{
    return std::forward<GeneratorT>(generator) | take(count);
}

} // namespace generator
} // namespace xtras

#endif // XTRAS_GENERATOR_TAKE_HPP_
