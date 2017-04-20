#ifndef RAFALW_GENERATOR_JOIN_HPP_
#define RAFALW_GENERATOR_JOIN_HPP_

#include <rafalw/generator/Base.hpp>
#include <rafalw/utils/static.hpp>
#include <rafalw/tuple.hpp>
#include <tuple>

inline namespace rafalw {
namespace generator {

template<typename... GeneratorsT>
class Join : private Base
{
public:
    template<typename... GeneratorsT2>
    Join(GeneratorsT2&&... generators) :
        m_generators{ std::forward<GeneratorsT2>(generators)... }
    {}

private:
    friend class BaseAccess;

    std::tuple<GeneratorsT...> m_generators;

    using References = std::array<ErasedReference<decltype(peek(std::get<0>(m_generators)))>, sizeof...(GeneratorsT)>;
    References m_references{{
        std::get<GeneratorsT>(m_generators)...
    }};

    typename References::iterator m_current = m_references.begin();

    auto generatorDone() const -> bool
    {
        return m_current == end(m_references);
    }

    decltype(auto) generatorPeek() const
    {
        return peek(*m_current);
    }

    auto generatorUpdate() -> void
    {
        update(*m_current);

        if (done(*m_current) && m_current != end(m_references))
            m_current++;
    }
};

template<typename... GeneratorsT>
auto join(GeneratorsT&&... generators) -> Join<std::decay_t<GeneratorsT>...>
{
    return Join<std::decay_t<GeneratorsT>...>{ std::forward<GeneratorsT>(generators)... };
}

} // namespace generator
} // namespace rafalw



#endif // RAFALW_GENERATOR_JOIN_HPP_
