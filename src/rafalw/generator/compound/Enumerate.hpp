#ifndef RAFALW_GENERATOR_COMPOUND_ENUMERATE_HPP_
#define RAFALW_GENERATOR_COMPOUND_ENUMERATE_HPP_

#include <rafalw/generator/simple/Sequence.hpp>
#include <rafalw/generator/compound/Zip.hpp>

inline namespace rafalw {
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
} // namespace rafalw

#endif // RAFALW_GENERATOR_COMPOUND_ENUMERATE_HPP_
