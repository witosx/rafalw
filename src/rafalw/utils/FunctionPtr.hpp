#ifndef RAFALW_UTILS_FUNCTIONPTR_HPP_
#define RAFALW_UTILS_FUNCTIONPTR_HPP_

inline namespace rafalw {
namespace utils {

namespace detail {

    template<typename T>
    struct FunctionPtr;

    template<typename R, typename... Args>
    struct FunctionPtr<R(Args...)>
    {
        using Type = R(*)(Args...);
    };

} // namespace detail

template<typename T>
using FunctionPtr = typename detail::FunctionPtr<T>::Type;

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_FUNCTIONPTR_HPP_
