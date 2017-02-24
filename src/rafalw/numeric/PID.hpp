#ifndef RAFALW_NUMERIC_PID_HPP_
#define RAFALW_NUMERIC_PID_HPP_

#include <rafalw/utils/assert.hpp>

inline namespace rafalw {
namespace numeric {

template<typename SampleT, typename TimeT = double>
class PID
{
public:
    using Time = TimeT;
    using Input = SampleT;
    using Output = Input;

    using Duration = decltype(std::declval<Time>() - std::declval<Time>());

    PID(Duration tau, double mp, double mi) :
        m_tau{ tau },
        m_multiplierP{ mp },
        m_multiplierI{ mi }
    {}

    auto update(Time itime, Input ivalue) -> void
    {
        if (!m_state)
        {
            m_state = State{ itime, ivalue, ivalue };
            return;
        }

        rafalw_utils_assert(!(itime < m_state->time));

        const auto dt = itime - m_state->time;
        const auto dv = ivalue - m_state->output;
        const auto diff = (dt / m_tau) * dv;

        m_integrated += diff;

        const auto dp = m_multiplierP * diff;
        const auto di = m_multiplierI * m_integrated;
        const auto correction = dp + di;

        m_state->time = itime;
        m_state->input = ivalue;
        m_state->output += correction;
    }

    auto update(Time time) -> void
    {
        if (!m_state)
            return;

        update(time, m_state->input);
    }

    auto value() const -> Output
    {
        return m_state->output;
    }

private:
    struct State
    {
        Time time;
        Input input;
        Output output;
    };

    boost::optional<State> m_state;

    Duration m_tau;
    double m_multiplierP;
    double m_multiplierI;

    Output m_integrated{ 0 };
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_PID_HPP_
