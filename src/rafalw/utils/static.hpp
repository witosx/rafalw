#ifndef RAFALW_UTILS_STATIC_HPP_
#define RAFALW_UTILS_STATIC_HPP_

#include <tuple>
#include <type_traits>
#include <utility>

inline namespace rafalw {
namespace utils {

template<typename T, T I, T END, T STEP, typename F>
constexpr auto static_for(F&& f) -> void;

template<typename T, T I, T END, typename F>
constexpr auto static_for(F&& f) -> void;

template<int I, int END, typename F>
constexpr auto static_for(F&& f) -> void;

template<typename T, typename F>
constexpr auto static_foreach(T&& c, F&& f) -> void;

template<typename T, typename F>
constexpr auto static_enumerate(T&& c, F&& f) -> void;

namespace static_detail {

    template<typename T, T I, T END, T STEP, typename F, std::enable_if_t<I >= END>* = nullptr>
    constexpr auto static_for_call(F&&) -> void
    {}

    template<typename T, T I, T END, T STEP, typename F, std::enable_if_t<I < END>* = nullptr>
    constexpr auto static_for_call(F&& f) -> void
    {
        f(std::integral_constant<T, I>{});
        static_for_call<T, I + STEP, END, STEP>(std::forward<F>(f));;
    }

} // namespace static_detail

template<typename T, T I, T END, T STEP, typename F>
constexpr auto static_for(F&& f) -> void
{
    static_detail::static_for_call<T, I, END, STEP>(std::forward<F>(f));
}

template<typename T, T I, T END, typename F>
constexpr auto static_for(F&& f) -> void
{
    static_for<T, I, END, 1>(std::forward<F>(f));
}

template<int I, int END, typename F>
constexpr auto static_for(F&& f) -> void
{
    static_for<int, I, END>(std::forward<F>(f));
}

template<typename T, typename F>
constexpr auto static_foreach(T&& t, F&& f) -> void
{
    using std::get;

    constexpr auto N = std::tuple_size<std::remove_reference_t<T>>::value;
    static_for<std::size_t, 0, N>([&t,&f](auto i){ f(get<i()>(t)); });
}

template<typename T, typename F>
constexpr auto static_enumerate(T&& t, F&& f) -> void
{
    using ::std::get;
    using T2 = decltype(t);

    constexpr auto N = std::tuple_size<std::remove_reference_t<T2>>::value;
    static_for<std::size_t, 0, N>([&t,&f](auto i){ f(i, get<i()>(std::forward<T2>(t))); });
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_STATIC_HPP_
