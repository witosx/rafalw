#ifndef XTRAS_UTILS_DATETIME_HPP_
#define XTRAS_UTILS_DATETIME_HPP_

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <cmath>

namespace xtras {
inline namespace utils {
namespace datetime {

using Date = boost::gregorian::date;
using DateTime = boost::posix_time::ptime;
using Duration = boost::posix_time::time_duration;

inline auto epoch() -> DateTime
{
    static const auto result = DateTime{ Date{ 1970, 1, 1 } };
    return result;
}

inline auto duration_null() -> Duration
{
    static const auto result = Duration{ 0, 0, 0 };
    return result;
}

inline auto duration_from_seconds(double seconds) -> Duration
{
    auto ipart = 0.0;
    auto fpart = std::modf(seconds, &ipart);
    return boost::posix_time::seconds{ static_cast<long>(ipart) } + boost::posix_time::microseconds{ static_cast<boost::int64_t>(fpart * 1e6) };
}

inline auto duration_to_seconds(Duration d) -> double
{
    return d.total_microseconds() / 1.0e6;
}

inline auto from_timestamp(double timestamp) -> DateTime
{
    return epoch() + duration_from_seconds(timestamp);
}

inline auto to_timestamp(DateTime time) -> double
{
    return duration_to_seconds(time - epoch());
}

} // namespace datetime
} // namespace utils
} // namespace xtras

#endif // XTRAS_UTILS_DATETIME_HPP_
