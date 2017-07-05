#ifndef XTRAS_GENERATOR_COMPOSITE_OPTIONAL_HPP_
#define XTRAS_GENERATOR_COMPOSITE_OPTIONAL_HPP_

#include <xtras/generator/Base.hpp>
#include <optional>
#include <type_traits>

namespace xtras {
namespace generator {

template<typename GeneratorT>
class Optional : private Base
{
public:
    using Generator = GeneratorT;

    Optional() = default;

    template<typename G, std::enable_if_t<!std::is_same<std::decay_t<G>, Optional<GeneratorT>>::value>* = nullptr>
    Optional(G&& generator) :
        m_generator{ std::forward<G>(generator) }
    {}

private:
    friend class BaseAccess;

    std::optional<Generator> m_generator;

    auto generatorDone() const -> bool
    {
        if (!m_generator)
            return true;

        return done(*m_generator);
    }

    decltype(auto) generatorPeek() const
    {
        return peek(*m_generator);
    }

    auto generatorUpdate() -> void
    {
        update(*m_generator);
    }

    auto generatorReset()
    {
        static constexpr auto RES = reset_ok<Generator>;

        if (RES && m_generator)
            try_reset(*m_generator);

        return RESET_TAG<RES>;
    }
};

template<typename GeneratorT>
auto optional() -> Optional<std::decay_t<GeneratorT>>
{
    return Optional<std::decay_t<GeneratorT>>{};
}

template<typename GeneratorT>
auto optional(GeneratorT&& generator) -> Optional<std::decay_t<GeneratorT>>
{
    return Optional<std::decay_t<GeneratorT>>{ std::forward<GeneratorT>(generator) };
}

} // namespace generator
} // namespace xtras



#endif // XTRAS_GENERATOR_COMPOSITE_OPTIONAL_HPP_
