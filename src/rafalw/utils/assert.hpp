#ifndef RAFALW_UTILS_ASSERT_HPP_
#define RAFALW_UTILS_ASSERT_HPP_

#include <rafalw/utils/Error.hpp>

inline namespace rafalw {
namespace utils {

class AssertionError : public Error
{
public:
    AssertionError(const std::string& file, int line, const char* condition) :
        Error{ "assertion failed - ", condition, " in ", file, ":", line }
    {}
};

} // namespace utils
} // namespace rafalw

#define rafalw_utils_assert(cond) \
    do { \
        if (!(cond)) \
            throw ::rafalw::utils::AssertionError{ __FILE__, __LINE__, #cond }; \
    } while (false)

#endif // RAFALW_UTILS_ASSERT_HPP_
