#ifndef RAFALW_GENERATOR_REFERENCE_HPP_
#define RAFALW_GENERATOR_REFERENCE_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename GeneratorT>
class Reference : private Base
{
public:
    using Generator = GeneratorT;

    Reference(ConstructTag, Generator& generator) :
        m_generator{ generator }
    {}

private:
    friend class BaseAccess;

    Generator& m_generator;

    auto generatorDone() const -> bool
    {
        return done(m_generator);
    }

    decltype(auto) generatorPeek() const
    {
        return peek(m_generator);
    }

    auto generatorUpdate() -> void
    {
        update(m_generator);
    }

    auto generatorReset()
    {
        return try_reset(m_generator);
    }
};

template<typename GeneratorT>
auto reference(GeneratorT& generator) -> Reference<GeneratorT>
{
    return Reference<GeneratorT>{ ConstructTag{}, generator };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_REFERENCE_HPP_
