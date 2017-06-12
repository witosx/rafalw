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
    template<typename T>
    constexpr Ignore(const T&)
    {}

    template<typename T>
    constexpr auto operator =(const T&) -> Ignore&
    {
        return *this;
    }
};

template<typename T>
struct Type
{
    using type = T;
};

template<typename T>
static constexpr auto type = Type<T>{};

template<typename T>
using identity = T;

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_HELPERS_HPP_
