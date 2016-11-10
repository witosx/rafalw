#ifndef RAFALW_UTILS_ASSERT_HPP_
#define RAFALW_UTILS_ASSERT_HPP_

#include <rafalw/utils/Error.hpp>
#include <cstdlib>

inline namespace rafalw {
namespace utils {

class AssertionError : public Error
{
public:
    AssertionError(const char* file, const char* func, int line, const char* condition) :
        Error{ "assertion `", condition, "' failed in ", file, ":", line, " [", func, "]" }
    {}
};

} // namespace utils
} // namespace rafalw

#if !defined(RAFALW_UTILS_ASSERT_MODE) || (RAFALW_UTILS_ASSERT_MODE == 0)
#define rafalw_utils_assert(cond)
#elif RAFALW_UTILS_ASSERT_MODE == 1
#define rafalw_utils_assert(cond) assert(cond)
#elif RAFALW_UTILS_ASSERT_MODE == 2
#define rafalw_utils_assert(cond) throw rafalw::utils::AssertionError{ __FILE__, __PRETTY_FUNCTION__, __LINE__, #cond }
#endif

#endif // RAFALW_UTILS_ASSERT_HPP_
