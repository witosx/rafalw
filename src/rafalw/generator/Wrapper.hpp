#ifndef RAFALW_GENERATOR_WRAPPER_HPP_
#define RAFALW_GENERATOR_WRAPPER_HPP_

#include <type_traits>

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
        update(generator);
    }

    auto wrapperReset()
    {
        return RESET_TAG<false>;
    }
};

template<typename T>
using IsWrapper = std::is_base_of<Wrapper, T>;

template<typename T>
static constexpr auto is_wrapper = IsWrapper<T>::value;

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_WRAPPER_HPP_
