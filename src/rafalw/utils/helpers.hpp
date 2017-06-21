#ifndef RAFALW_UTILS_HELPERS_HPP_
#define RAFALW_UTILS_HELPERS_HPP_

#include <utility>
#include <functional>

inline namespace rafalw {
namespace utils {

struct Invoke
{
    template<typename Function>
    constexpr Invoke(Function&& function)
    {
        std::invoke(std::forward<Function>(function));
    }
};

struct Ignore
{
    constexpr Ignore() = default;

    template<typename T>
    constexpr Ignore(const T&)
    {}

    template<typename T>
    constexpr auto operator =(const T&) -> Ignore&
    {
        return *this;
    }
};

inline constexpr auto ignore = Ignore{};

template<typename T>
struct WrappedType
{
    using type = T;
};

template<typename T>
constexpr auto wrap() -> WrappedType<T>
{
    return WrappedType<T>{};
}


template<auto V>
struct WrappedValue
{
    static constexpr auto value = V;
};

template<auto V>
constexpr auto wrap() -> WrappedValue<V>
{
    return WrappedValue<V>{};
}


template<typename T>
using identity = T;

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_HELPERS_HPP_
