#ifndef XTRAS_UTILS_ARRAY_HPP_
#define XTRAS_UTILS_ARRAY_HPP_

#include <array>
#include <type_traits>

namespace xtras {
inline namespace utils {

namespace _impl {
    template<typename T, typename... Args>
    using ArrayTypeHelper = std::conditional_t<std::is_same<T, void>::value, std::common_type_t<Args...>, T>;
}

template<typename T = void, typename... Args>
constexpr auto make_array(Args&&... args) -> std::array<_impl::ArrayTypeHelper<T, Args...>, sizeof...(Args)>;

template<typename T, typename... Args>
constexpr auto make_array(Args&&... args) -> std::array<_impl::ArrayTypeHelper<T, Args...>, sizeof...(Args)>
{
    using E = _impl::ArrayTypeHelper<T, Args...>;
    return std::array<E, sizeof...(Args)>{{ std::forward<Args>(args)... }};
}

} // namespace utils
} // namespace xtras

#endif // XTRAS_UTILS_ARRAY_HPP_
