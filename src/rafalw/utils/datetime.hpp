#ifndef RAFALW_UTILS_DATETIME_HPP_
#define RAFALW_UTILS_DATETIME_HPP_

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <rafalw/generator/Base.hpp>
#include <cmath>

inline namespace rafalw {
namespace utils {
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


class DateRange : private generator::Base
{
public:
    DateRange(const Date& date_beg, const Date& date_end) :
        m_date{ date_beg },
        m_dateEnd{ date_end }
    {}

private:
    friend class generator::BaseAccess;

    boost::gregorian::date_duration m_step = boost::gregorian::days{ 1 };

    Date m_date;
    Date m_dateEnd;

    auto generatorDone() const -> bool
    {
        return m_date >= m_dateEnd;
    }

    auto generatorPeek() const -> const Date&
    {
        return m_date;
    }

    auto generatorUpdate() -> void
    {
        m_date += m_step;
    }
};

} // namespace datetime
} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_DATETIME_HPP_
