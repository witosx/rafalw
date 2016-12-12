#ifndef RAFALW_UTILS_ASSERT_HPP_
#define RAFALW_UTILS_ASSERT_HPP_

#include <rafalw/utils/Error.hpp>
#include <cstdlib>
#include <iostream>

inline namespace rafalw {
namespace utils {

class AssertionError : public Error
{
public:
    AssertionError(const char* condition, const char* file, const char* func, int line) :
        Error{ "assertion `", condition, "' failed in ", file, ":", line, " [", func, "]" }
    {}
};

inline auto assertion_check_throw(bool cond, const char* cond_str, const char* file, const char* func, int line) -> void
{
    if (!cond)
        throw AssertionError{ cond_str, file, func, line };
}

inline auto assertion_check_abort(bool cond, const char* cond_str, const char* file, const char* func, int line) -> void
{
    if (!cond)
    {
        std::cerr << "assertion `" << cond_str <<  "' failed in " <<  file <<  ":" <<  line <<  " [" <<  func <<  "]" << std::endl;
        std::abort();
    }
}

} // namespace utils
} // namespace rafalw

#if !defined(RAFALW_UTILS_ASSERT_MODE)
#define RAFALW_UTILS_ASSERT_MODE 2
#endif

#if RAFALW_UTILS_ASSERT_MODE == 0
#define rafalw_utils_assert(cond)
#elif RAFALW_UTILS_ASSERT_MODE == 1
#define rafalw_utils_assert(cond) ::rafalw::utils::assertion_check_abort(static_cast<bool>(cond), #cond, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#elif RAFALW_UTILS_ASSERT_MODE == 2
#define rafalw_utils_assert(cond) ::rafalw::utils::assertion_check_throw(static_cast<bool>(cond), #cond, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#endif

#endif // RAFALW_UTILS_ASSERT_HPP_
