#ifndef XTRAS_UTILS_ASSERT_HPP_
#define XTRAS_UTILS_ASSERT_HPP_

#include <xtras/utils/Error.hpp>
#include <cstdlib>
#include <iostream>

namespace xtras {
inline namespace utils {

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
} // namespace xtras

#if !defined(XTRAS_ASSERT_MODE)
    #ifdef NDEBUG
        #define XTRAS_ASSERT_MODE 0
    #else
        #define XTRAS_ASSERT_MODE 2
    #endif
#endif

#if XTRAS_ASSERT_MODE == 0
    #define xtras_assert(cond)
#elif XTRAS_ASSERT_MODE == 1
    #define xtras_assert(cond) ::xtras::assertion_check_abort(static_cast<bool>(cond), #cond, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#elif XTRAS_ASSERT_MODE == 2
    #define xtras_assert(cond) ::xtras::assertion_check_throw(static_cast<bool>(cond), #cond, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#endif

#endif // XTRAS_UTILS_ASSERT_HPP_
