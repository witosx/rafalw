#ifndef XTRAS_UTILS_TIMEIT_HPP_
#define XTRAS_UTILS_TIMEIT_HPP_

#include <type_traits>
#include <chrono>
#include <utility>

namespace xtras {
inline namespace utils {

template<typename _Duration, typename _Result>
struct TimeItBase
{
    using Duration = _Duration;
    using Result = _Result;

    Duration duration;
};

template<typename _Duration, typename _Result>
struct TimeIt : public TimeItBase<_Duration, _Result>
{
    using Duration = typename TimeIt<_Duration, _Result>::Duration;
    using Result = typename TimeIt<_Duration, _Result>::Result;

    Result result;

    TimeIt(Duration d, Result&& r) :
        TimeItBase<_Duration, _Result>{ d },
        result{ std::move(r) }
    {}
};

template<typename _Duration>
struct TimeIt<_Duration, void> : public TimeItBase<_Duration, void>
{
    using Duration = typename TimeIt<_Duration, void>::Duration;

    TimeIt(Duration d) :
        TimeItBase<_Duration, void>{ d }
    {}
};

namespace detail {

    template<typename Clock, typename T, typename R>
    auto result(R&& res, const typename Clock::time_point& tp1) -> TimeIt<typename Clock::duration, T>
    {
        auto tp2 = Clock::now();
        return TimeIt<typename Clock::duration, T>{ tp2 - tp1, std::forward<R>(res) };
    }

    template<typename Clock, typename F, std::enable_if_t<std::is_same<decltype(std::declval<F>()()), void>::value>* = nullptr>
    auto timeit(F&& f) -> TimeIt<typename Clock::duration, decltype(f())>
    {
        auto tp1 = Clock::now();
        std::forward<F>(f)();
        auto tp2 = Clock::now();
        return TimeIt<typename Clock::duration, decltype(f())>{ tp2 - tp1 };
    }

    template<typename Clock, typename F, std::enable_if_t<!std::is_same<decltype(std::declval<F>()()), void>::value>* = nullptr>
    auto timeit(F&& f) -> TimeIt<typename Clock::duration, decltype(f())>
    {
        const auto tp1 = Clock::now();
        return result<Clock, decltype(f())>(std::forward<F>(f)(), tp1);
    }
}

template<typename Clock, typename F>
auto timeit_clock(F&& f) -> decltype(detail::timeit<Clock>(std::forward<F>(f)))
{
    return detail::timeit<Clock>(std::forward<F>(f));
}

template<typename F>
auto timeit(F&& f) -> decltype(timeit_clock<std::chrono::steady_clock>(std::forward<F>(f)))
{
    return timeit_clock<std::chrono::steady_clock>(std::forward<F>(f));
}

} // namespace utils
} // namespace xtras

#endif // XTRAS_UTILS_TIMEIT_HPP_
