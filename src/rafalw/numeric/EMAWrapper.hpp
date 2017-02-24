#ifndef RAFALW_NUMERIC_EMAWRAPPER_HPP_
#define RAFALW_NUMERIC_EMAWRAPPER_HPP_

#include <rafalw/numeric/EMA.hpp>
#include <boost/optional.hpp>
#include <utility>

inline namespace rafalw {
namespace numeric {

enum class EMAMode
{
    PRE,
    POST
};

template<typename SampleT, typename TimeT = double>
class EMAWrapper
{
public:
    using Sample = SampleT;
    using Time = TimeT;
    using Duration = decltype(std::declval<Time>() - std::declval<Time>());
    using UnderlyingEMA = numeric::EMA<Sample, Duration>;
    using Value = typename UnderlyingEMA::Value;

    EMAWrapper(EMAMode mode, EMAType type, Duration tau) :
        m_mode{ mode },
        m_ema{ type, tau }
    {}

    EMAWrapper(EMAMode mode, EMAType type, Duration tau_grow, Duration tau_reduce) :
        m_mode{ mode },
        m_ema{ type, tau_grow, tau_reduce }
    {}

    auto initialized() const -> bool
    {
        return m_ema.initialized();
    }

    auto initialize(Value sample) -> void
    {
        m_ema.initialize(sample);
    }

    auto value() const -> Value
    {
        return m_ema.value();
    }

    auto updateTau(Duration tau) -> void
    {
        m_ema.updateTau(tau);
    }

    auto updateTau(Duration tau_grow, Duration tau_reduce) -> void
    {
        m_ema.updateTau(tau_grow, tau_reduce);
    }

    auto update(Time time, Sample sample) -> void
    {
        if (!m_data)
            m_data = Data{ time, sample };

        const auto s = (m_mode == EMAMode::PRE) ? sample : m_data->sample;

        doUpdate(time, s);
        m_data->sample = sample;
    }

    auto update(Time time) -> void
    {
        if (!m_data)
            return;

        doUpdate(time, m_data->sample);
    }

private:
    struct Data
    {
        Time time;
        Sample sample;
    };

    EMAMode m_mode;
    UnderlyingEMA m_ema;
    boost::optional<Data> m_data;

    auto doUpdate(Time time, Sample sample) -> void
    {
        rafalw_utils_assert(m_data);
        rafalw_utils_assert(time >= m_data->time);

        m_ema.update(sample, time - m_data->time);
        m_data->time = time;
    }
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMAWRAPPER_HPP_
