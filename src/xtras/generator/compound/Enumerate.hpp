#ifndef XTRAS_GENERATOR_COMPOUND_ENUMERATE_HPP_
#define XTRAS_GENERATOR_COMPOUND_ENUMERATE_HPP_

#include <xtras/generator/simple/Sequence.hpp>
#include <xtras/generator/compound/Zip.hpp>

namespace xtras {
namespace generator {

template<typename Generator, typename Index>
auto enumerate(Generator&& g, Index begin)
{
    return zip(std::forward<Generator>(g), generator::sequence(begin, std::numeric_limits<Index>::max()));
}

template<typename Generator>
auto enumerate(Generator&& g)
{
    return enumerate(std::forward<Generator>(g), 0);
}

} // namespace generator
} // namespace xtras

#endif // XTRAS_GENERATOR_COMPOUND_ENUMERATE_HPP_
