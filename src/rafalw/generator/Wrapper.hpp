#ifndef RAFALW_GENERATOR_WRAPPER_HPP_
#define RAFALW_GENERATOR_WRAPPER_HPP_

inline namespace rafalw {
namespace generator {

class Wrapper
{
public:
    template<typename GeneratorT>
    auto wrapperInit(GeneratorT& /*generator*/) -> void
    {}

    template<typename GeneratorT>
    auto wrapperDone(const GeneratorT& generator) const -> bool
    {
        return done(generator);
    }

    template<typename GeneratorT>
    auto wrapperPeek(const GeneratorT& generator) const -> decltype(peek(generator))
    {
        return peek(generator);
    }

    template<typename GeneratorT>
    auto wrapperUpdate(GeneratorT& generator) -> void
    {
        return update(generator);
    }

    auto wrapperReset()
    {
        return RESET_UNAVAILABLE;
    }
};

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_WRAPPER_HPP_
