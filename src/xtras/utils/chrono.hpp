#ifndef XTRAS_UTILS_CHRONO_HPP_
#define XTRAS_UTILS_CHRONO_HPP_

#include <type_traits>
#include <chrono>
#include <utility>

namespace xtras {
inline namespace utils {

namespace detail {

    template<typename Duration, typename NewRep>
    struct DurationChangeRepTrait;

    template<typename Rep, typename Period, typename NewRep>
    struct DurationChangeRepTrait<std::chrono::duration<Rep, Period>, NewRep>
    {
        using Type = std::chrono::duration<NewRep, Period>;
    };

    template<typename Duration, typename NewRep>
    using DurationChangeRep = typename DurationChangeRepTrait<Duration, NewRep>::Type;


} // namespace detail

using HoursFP = detail::DurationChangeRep<std::chrono::hours, double>;
using MinutesFP = detail::DurationChangeRep<std::chrono::minutes, double>;
using SecondsFP = detail::DurationChangeRep<std::chrono::seconds, double>;
using MillisecondsFP = detail::DurationChangeRep<std::chrono::milliseconds, double>;
using MicrosecondsFP = detail::DurationChangeRep<std::chrono::microseconds, double>;
using NanosecondsFP = detail::DurationChangeRep<std::chrono::nanoseconds, double>;

} // namespace utils
} // namespace xtras

#endif // XTRAS_UTILS_CHRONO_HPP_
