#ifndef XTRAS_UTILS_INTEGER_CAST_HPP_
#define XTRAS_UTILS_INTEGER_CAST_HPP_

#include <xtras/utils/assert.hpp>
#include <limits>
#include <type_traits>

namespace xtras {
inline namespace utils {

template<typename Target, typename Source>
auto integer_cast(Source s) -> Target
{
    static_assert(std::is_integral<Target>::value, "target type must be integral");
    static_assert(std::is_integral<Source>::value, "source type must be integral");

    xtras_assert(s >= std::numeric_limits<Target>::min());
    xtras_assert(s <= std::numeric_limits<Target>::max());

    return static_cast<Target>(s);
}

} //
} //

#endif // XTRAS_UTILS_INTEGER_CAST_HPP_
