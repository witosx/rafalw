#ifndef XTRAS_UTILS_TIMEPERIODS_HPP_
#define XTRAS_UTILS_TIMEPERIODS_HPP_

#include <xtras/utils/assert.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <algorithm>
#include <vector>

namespace xtras {
inline namespace utils {

class TimePeriods
{
public:
    using Time = boost::posix_time::ptime;
    using Duration = decltype(std::declval<Time>() - std::declval<Time>());

    struct Period
    {
        Time beginTime;
        Time endTime;
        bool active;

        Period(Time begin_time, Time end_time, bool active) :
            beginTime{ begin_time },
            endTime{ end_time },
            active{ active }
        {}

        auto duration() const -> Duration
        {
            return endTime - beginTime;
        }

        auto contains(Time t) const -> bool
        {
            return t >= beginTime && t < endTime;
        }

        auto first() const -> bool
        {
            return beginTime == timeMin();
        }

        auto last() const -> bool
        {
            return endTime == timeMax();
        }

        auto special() const -> bool
        {
            return first() || last();
        }
    };

    using Data = std::vector<Period>;
    using Iterator = Data::const_iterator;

    static auto timeMin() -> Time
    {
        return boost::posix_time::neg_infin;
    }

    static auto timeMax() -> Time
    {
        return boost::posix_time::pos_infin;
    }

    TimePeriods()
    {
        reset();
    }

    auto append(Time begin_time, bool active) -> void
    {
        xtras_assert(begin_time >= timeMin());
        xtras_assert(begin_time <= timeMax());

        if (begin_time == timeMax())
            return;

        auto& p = m_data;

        xtras_assert(!p.empty());
        xtras_assert(!(begin_time < p.back().beginTime));

        if (p.back().active == active)
            return;

        if (p.back().beginTime < begin_time)
        {
            p.back().endTime = begin_time;
            p.emplace_back(begin_time, timeMax(), active);
        }
        else // p.back().beginTime == t
        {
            if (p.size() > 1)
            {
                p.pop_back();
                p.back().endTime = timeMax();
            }
            else
            {
                p.back().active = active;
            }
        }
    }

    auto reset() -> void
    {
        m_data.clear();
        m_data.emplace_back(timeMin(), timeMax(), false);
    }

    auto invert() -> void
    {
        for (auto&& p: m_data)
            p.active = !p.active;
    }

    auto data() const -> const Data&
    {
        xtras_assert(!m_data.empty());
        return m_data;
    }

    auto appendActive(Time begin_time) -> void
    {
        append(begin_time, true);
    }

    auto appendInactive(Time begin_time) -> void
    {
        append(begin_time, false);
    }

    auto begin() const -> Iterator
    {
        return data().begin();
    }

    auto end() const -> Iterator
    {
        return data().end();
    }

    auto front() const -> const Period&
    {
        return data().front();
    }

    auto back() const -> const Period&
    {
        return data().back();
    }

    auto size() const -> std::size_t
    {
        return data().size();
    }

    auto find(Time t) const -> Iterator
    {
        xtras_assert(t >= timeMin());
        xtras_assert(t <= timeMax());

        auto it = std::lower_bound(data().begin(), data().end(), t, [](const Period& p, Time t) {
            return p.endTime <= t;
        });

        xtras_assert(it != data().end());
        xtras_assert(t >= it->beginTime && t < it->endTime);

        return it;
    }

    auto at(Time t) const -> const Period&
    {
        return *find(t);
    }

private:
    Data m_data;
};

namespace detail {

    template<typename F>
    inline auto time_periods_process(const TimePeriods& tps1, const TimePeriods& tps2, F&& f) -> void
    {
        auto it1 = tps1.begin();
        auto it2 = tps2.begin();

        while (true)
        {
            xtras_assert(it1 != tps1.end());
            xtras_assert(it2 != tps2.end());

            f(*it1, *it2);

            if (it1->last() && it2->last())
                break;

            const auto et = std::min(it1->endTime, it2->endTime);

            if (!it1->last() && it1->endTime == et)
                it1++;

            if (!it2->last() && it2->endTime == et)
                it2++;
        }
    }

    template<typename F>
    inline auto time_periods_combine(const TimePeriods& tps1, const TimePeriods& tps2, F&& f) -> TimePeriods
    {
        auto res = TimePeriods{};

        detail::time_periods_process(tps1, tps2, [&res,&f](const auto& p1, const auto& p2){
            if (f(p1.active, p2.active)) {
                res.appendActive(std::max(p1.beginTime, p2.beginTime));
                res.appendInactive(std::min(p1.endTime, p2.endTime));
            }
        });

        return res;
    }

} // namespace detail

inline auto operator ~(TimePeriods tps) -> TimePeriods
{
    tps.invert();
    return tps;
}

inline auto operator &(const TimePeriods& tps1, const TimePeriods& tps2) -> TimePeriods
{
    return detail::time_periods_combine(tps1, tps2, [](bool a1, bool a2){
        return a1 && a2;
    });
}

inline auto operator -(const TimePeriods& tps1, const TimePeriods& tps2) -> TimePeriods
{
    return detail::time_periods_combine(tps1, tps2, [](bool a1, bool a2){
        return a1 && !a2;
    });
}

inline auto operator ^(const TimePeriods& tps1, const TimePeriods& tps2) -> TimePeriods
{
    return detail::time_periods_combine(tps1, tps2, [](bool a1, bool a2){
        return a1 != a2;
    });
}

inline auto operator |(const TimePeriods& tps1, const TimePeriods& tps2) -> TimePeriods
{
    return detail::time_periods_combine(tps1, tps2, [](bool a1, bool a2){
        return a1 || a2;
    });
}

} // namespace utils
} // namespace xtras

#endif // XTRAS_UTILS_TIMEPERIODS_HPP_
