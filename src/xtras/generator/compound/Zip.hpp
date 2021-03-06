#ifndef XTRAS_GENERATOR_ZIP_HPP_
#define XTRAS_GENERATOR_ZIP_HPP_

#include <xtras/generator/Base.hpp>
#include <xtras/utils/static.hpp>
#include <tuple>

namespace xtras {
namespace generator {

template<typename... GeneratorsT>
class Zip : private Base
{
public:
    template<typename... GeneratorsT2>
    Zip(ConstructTag, GeneratorsT2&&... generators) :
        m_generators{ std::forward<GeneratorsT2>(generators)... }
    {}

private:
    friend class BaseAccess;

    std::tuple<GeneratorsT...> m_generators;

    auto generatorDone() const -> bool
    {
        return std::apply([](const auto&... gens){
            return (done(gens) || ...);
        }, m_generators);
    }

    auto generatorPeek() const
    {
        return std::apply([](const auto&... gens){
            return std::tuple<decltype(peek(gens))...>{ peek(gens)... };
        }, m_generators);
    }

    auto generatorUpdate() -> void
    {
        static_foreach(m_generators, [](auto& gen){
            update(gen);
        });
    }

    auto generatorReset()
    {
        static constexpr auto OK = std::conjunction_v<reset_tag<GeneratorsT>...>;

        if (OK)
        {
            static_foreach(m_generators, [](auto& gen){
                try_reset(gen);
            });
        }

        return RESET_TAG<OK>;
    }
};

template<typename... GeneratorsT>
auto zip(GeneratorsT&&... generators) -> Zip<std::decay_t<GeneratorsT>...>
{
    return Zip<std::decay_t<GeneratorsT>...>{ ConstructTag{}, std::forward<GeneratorsT>(generators)... };
}

} // namespace generator
} // namespace xtras

#endif // XTRAS_GENERATOR_ZIP_HPP_
