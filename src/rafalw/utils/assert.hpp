#ifndef RAFALW_UTILS_ASSERT_HPP_
#define RAFALW_UTILS_ASSERT_HPP_

#include <rafalw/utils/Error.hpp>
#include <cstdlib>

#if !defined(RAFALW_UTILS_ASSERT_MODE) || (RAFALW_UTILS_ASSERT_MODE < 0) || (RAFALW_UTILS_ASSERT_MODE > 2)
#undef RAFALW_UTILS_ASSERT_MODE
#define RAFALW_UTILS_ASSERT_MODE 0
#endif

inline namespace rafalw {
namespace utils {

class AssertionError : public Error
{
public:
    AssertionError(const char* file, int line, const char* condition) :
        Error{ "assertion failed - ", condition, " in ", file, ":", line }
    {}
};

namespace detail {

    enum class AssertionMode : char
    {
        DISABLED = 0,
        ABORT = 1,
        THROW = 2
    };

    template<AssertionMode T>
    struct AssertionHandler;

    template<>
    struct AssertionHandler<AssertionMode::DISABLED>
    {
        static auto failed(const char* /*file*/, int /*line*/, const char* /*cond*/) -> void
        {}
    };

    template<>
    struct AssertionHandler<AssertionMode::ABORT>
    {
        static auto failed(const char* /*file*/, int /*line*/, const char* /*cond*/) -> void
        {
            std::abort();
        }
    };

    template<>
    struct AssertionHandler<AssertionMode::THROW>
    {
        static auto failed(const char* file, int line, const char* cond) -> void
        {
            throw AssertionError{ file, line, cond };
        }
    };

    #if (RAFALW_UTILS_ASSERT_MODE == 0)
        #define rafalw_utils_detail_assertion_check_failed(cond) false
    #else
        #define rafalw_utils_detail_assertion_check_failed(cond) !(cond)
    #endif

    constexpr auto ASSERTION_MODE = static_cast<AssertionMode>(RAFALW_UTILS_ASSERT_MODE);

    auto assertion_failed(const char* file, int line, const char* cond) -> void
    {
        AssertionHandler<ASSERTION_MODE>::failed(file, line, cond);
    }
}

} // namespace utils
} // namespace rafalw

#define rafalw_utils_assert(cond) \
    do { \
        if (rafalw_utils_detail_assertion_check_failed(cond)) \
            ::rafalw::utils::detail::assertion_failed(__FILE__, __LINE__, #cond); \
    } while (false)

#endif // RAFALW_UTILS_ASSERT_HPP_
