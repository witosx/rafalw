#ifndef XTRAS_UTILS_FUNCTIONPTR_HPP_
#define XTRAS_UTILS_FUNCTIONPTR_HPP_

namespace xtras {
inline namespace utils {

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
} // namespace xtras

#endif // XTRAS_UTILS_FUNCTIONPTR_HPP_
