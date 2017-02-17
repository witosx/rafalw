#ifndef RAFALW_UTILS_INVOKE_HPP_
#define RAFALW_UTILS_INVOKE_HPP_

#include <utility>

inline namespace rafalw {
namespace utils {

struct Invoke
{
    template<typename F>
    Invoke(F&& f)
    {
        std::forward<F>(f)();
    }
};

} // namespace utils
} // namespace rafalw



#endif // RAFALW_UTILS_INVOKE_HPP_
