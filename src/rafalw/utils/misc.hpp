#ifndef RAFALW_UTILS_MISC_HPP_
#define RAFALW_UTILS_MISC_HPP_

#include <iterator>
#include <functional>
#include <optional>

inline namespace rafalw {
namespace utils {

template<typename T>
auto last_element_iterator(T&& seq)
{
    using std::rbegin;
    using std::rend;
    using std::end;

    auto rit = rbegin(seq);

    if (rit == rend(seq))
        return end(seq);

    return std::next(rit).base();
}

template<typename Collection, typename... Args>
auto invoke_all(const Collection& collection, Args&&... args) -> void
{
    for (auto& f: collection)
        f(std::forward<Args>(args)...);
}

template<typename T>
constexpr auto hash(const T& o) -> std::size_t
{
    return std::hash<T>{}(o);
}

template<typename T>
constexpr auto round_int_up(T value, T base) -> T
{
    const auto res = value / base * base;
    return res < value ? res + base : res;
}

template<typename Int, typename T>
constexpr auto round_thr(T vf, T thr) -> Int
{
    const auto vi = static_cast<Int>(vf);

    if (vf - vi > thr)
        return vi + Int{ 1 };

    return vi;
}

template<typename T, typename F>
auto eval(T&& val, F&& func) -> std::optional<decltype(func(std::forward<T>(val)))>
{
    if (static_cast<bool>(std::forward<T>(val)))
        return std::optional<decltype(func(std::forward<T>(val)))>{ func(std::forward<T>(val)) };
    else
        return std::optional<decltype(func(std::forward<T>(val)))>{};
}

template<typename T>
auto clip(const T& v, const T& vmin, const T& vmax) -> T
{
    rafalw_utils_assert(!(vmax < vmin));

    if (v < vmin)
        return vmin;

    if (vmax < v)
        return vmax;

    return v;
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_MISC_HPP_
