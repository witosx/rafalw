#ifndef XTRAS_GENERATOR_SIMPLE_EMPTY_HPP_
#define XTRAS_GENERATOR_SIMPLE_EMPTY_HPP_

#include <xtras/generator/Base.hpp>

namespace xtras {
namespace generator {

template<typename ValueT>
class Empty : private Base
{
private:
    friend class BaseAccess;

    using Value = ValueT;

    auto generatorDone() const -> bool
    {
        return true;
    }

    auto generatorPeek [[noreturn]] () const -> Value
    {
        throw Error{ "xtras::generator::Empty peek not implemented" };
    }

    auto generatorUpdate() -> void
    {
        // nothing to do
    }

    auto generatorReset()
    {
        return RESET_TAG<true>;
    }
};

template<typename T>
static constexpr auto empty() -> Empty<T>
{
    return Empty<T>{};
}

} // namespace generator
} // namespace xtras

#endif // XTRAS_GENERATOR_SIMPLE_EMPTY_HPP_
