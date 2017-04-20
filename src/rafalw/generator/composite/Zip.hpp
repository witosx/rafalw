#ifndef RAFALW_GENERATOR_ZIP_HPP_
#define RAFALW_GENERATOR_ZIP_HPP_

#include <rafalw/generator/Base.hpp>
#include <rafalw/utils/static.hpp>
#include <rafalw/tuple.hpp>
#include <tuple>

inline namespace rafalw {
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
        return tuple::apply([](const auto&... gens){
            return (done(gens) || ...);
        }, m_generators);
    }

    auto generatorPeek() const
    {
        return tuple::apply([](const auto&... gens){
            return std::tuple<decltype(peek(gens))...>{ peek(gens)... };
        }, m_generators);
    }

    auto generatorUpdate() -> void
    {
        utils::static_foreach(m_generators, [](auto& gen){
            update(gen);
        });
    }

    auto generatorReset()
    {
        constexpr auto ok = std::conjunction<std::bool_constant<has_reset<GeneratorsT>>...>::value;

        if constexpr (ok)
        {
            utils::static_foreach(m_generators, [](auto& gen){
                reset(gen);
            });
            return;
        }
        else
        {
            return ResetNotImplemented{};
        }
    }
};

template<typename... GeneratorsT>
auto zip(GeneratorsT&&... generators) -> Zip<std::decay_t<GeneratorsT>...>
{
    return Zip<std::decay_t<GeneratorsT>...>{ ConstructTag{}, std::forward<GeneratorsT>(generators)... };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_ZIP_HPP_
