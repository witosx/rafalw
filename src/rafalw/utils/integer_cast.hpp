#ifndef RAFALW_UTILS_INTEGER_CAST_HPP_
#define RAFALW_UTILS_INTEGER_CAST_HPP_

#include <rafalw/utils/assert.hpp>
#include <limits>
#include <type_traits>

inline namespace rafalw {
namespace utils {

template<typename Target, typename Source>
auto integer_cast(Source s) -> Target
{
    static_assert(std::is_integral<Target>::value, "target type must be integral");
    static_assert(std::is_integral<Source>::value, "source type must be integral");

    rafalw_utils_assert(s >= std::numeric_limits<Target>::min());
    rafalw_utils_assert(s <= std::numeric_limits<Target>::max());

    return static_cast<Target>(s);
}

} //
} //

#endif // RAFALW_UTILS_INTEGER_CAST_HPP_
