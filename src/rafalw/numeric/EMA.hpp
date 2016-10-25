#ifndef RAFALW_NUMERIC_EMA_HPP_
#define RAFALW_NUMERIC_EMA_HPP_

#include <rafalw/utils/assert.hpp>
#include <boost/optional.hpp>
#include <cmath>
#include <utility>

inline namespace rafalw {
namespace numeric {

enum class EMAMode
{
    PRE,
    POST
};

template<typename _Alpha, typename _Value>
class BasicEMA
{
public:
    using Alpha = _Alpha;
    using Value = _Value;

private:
    struct Data
    {
        Alpha m_alpha;
        Value m_sample;
        Value m_value;
    };

    EMAMode m_mode;

    boost::optional<Data> m_data;

    auto newValue(Alpha alpha, Value sample) const -> Value
    {
        return (alpha * m_data->m_value) + (1 - alpha) * sample;;
    }

public:
    BasicEMA(EMAMode mode = EMAMode::POST) :
        m_mode{ mode }
    {}

    auto initialized() const -> bool
    {
        return static_cast<bool>(m_data);
    }

    auto value() const -> Value
    {
        rafalw_utils_assert(initialized());
        return m_data->m_value;
    }

    auto valueOr(const Value& vdef) const -> Value
    {
        if (!m_data)
            return vdef;

        return m_data->m_value;
    }

    auto mode() const -> EMAMode
    {
        return m_mode;
    }

    auto initialize(Value sample) -> void
    {
        m_data = Data{ 1, sample, sample };
    }

    auto update(Alpha alpha, Value sample) -> void
    {
        if (!initialized())
        {
            initialize(sample);
            return;
        }

        rafalw_utils_assert(alpha >= 0 && alpha <= 1);

        if (m_mode == EMAMode::PRE)
            m_data->m_value = newValue(m_data->m_alpha, m_data->m_sample);
        else
            m_data->m_value = newValue(alpha, sample);

        m_data->m_alpha = alpha;
        m_data->m_sample = sample;
    }
};

template<typename _Time, typename _Alpha, typename _Value>
class BasicSingleEMA
{
private:
    using EMA = BasicEMA<_Alpha, _Value>;

public:
    using Time = _Time;
    using TimeDelta = decltype(std::declval<Time>() - std::declval<Time>());
    using Value = typename EMA::Value;

private:
    TimeDelta m_tauGrow = std::numeric_limits<TimeDelta>::min();
    TimeDelta m_tauReduce = std::numeric_limits<TimeDelta>::min();

    EMA m_ema;
    Time m_time{};

public:
    BasicSingleEMA(EMAMode mode) :
        m_ema{ mode }
    {}

    auto tauGrowSet(TimeDelta tau) -> void
    {
        rafalw_utils_assert(m_tauGrow >= 0);
        m_tauGrow = tau;
    }

    auto tauReduceSet(TimeDelta tau) -> void
    {
        rafalw_utils_assert(m_tauReduce >= 0);
        m_tauReduce = tau;
    }

    auto initialize(Time time, Value sample) -> void
    {
        rafalw_utils_assert(time >= 0);
        m_ema.initialize(sample);
        m_time = time;
    }

    auto update(Time time, Value sample) -> void
    {
        if (!initialized())
        {
            initialize(time, sample);
            return;
        }

        rafalw_utils_assert(time > m_time);

        const auto dt = time - m_time;
        const auto tau = (sample > value() ? m_tauGrow : m_tauReduce);
        const auto alpha = std::exp(-dt / tau);

        m_ema.update(alpha, sample);
        m_time = time;
    }

    auto value() const -> Value
    {
        return m_ema.value();
    }

    auto valueOr(const Value& vdef) const -> Value
    {
        return m_ema.valueOr(vdef);
    }

    auto initialized() const -> bool
    {
        return m_ema.initialized();
    }
};

template<typename _Time, typename _Alpha, typename _Value>
class BasicDoubleEMA
{
private:
    using SEMA = BasicSingleEMA<_Time, _Alpha, _Value>;

public:
    using Time = typename SEMA::Time;
    using TimeDelta = typename SEMA::TimeDelta;
    using Value = typename SEMA::Value;

private:
    SEMA m_sema;
    SEMA m_sema2;

public:
    BasicDoubleEMA(EMAMode mode) :
        m_sema{ mode },
        m_sema2{ EMAMode::POST }
    {}

    auto tauSet(TimeDelta tau) -> void
    {
        m_sema.tauGrowSet(tau);
        m_sema.tauReduceSet(tau);

        m_sema2.tauGrowSet(tau);
        m_sema2.tauReduceSet(tau);
    }

    auto update(Time time, Value sample) -> void
    {
        m_sema.update(time, sample);
        m_sema2.update(time, sema());
    }

    auto value() const -> Value
    {
        return 2 * sema() - sema2();
    }

    auto sema() const -> Value
    {
        return m_sema.value();
    }

    auto sema2() const -> Value
    {
        return m_sema2.value();
    }

    auto initialized() const -> bool
    {
        return m_sema.initialized();
    }
};

template<typename _Time, typename _Alpha, typename _Value>
class BasicTripleEMA
{
private:
    using SEMA = BasicSingleEMA<_Time, _Alpha, _Value>;
    using DEMA = BasicDoubleEMA<_Time, _Alpha, _Value>;

public:
    using Time = typename SEMA::Time;
    using TimeDelta = typename SEMA::TimeDelta;
    using Value = typename SEMA::Value;

private:
    DEMA m_dema;
    SEMA m_sema3;

public:
    BasicTripleEMA(EMAMode mode) :
        m_dema{ mode },
        m_sema3{ EMAMode::POST }
    {}

    auto tauSet(TimeDelta tau) -> void
    {
        m_dema.tauGrowSet(tau);
        m_dema.tauReduceSet(tau);

        m_sema3.tauGrowSet(tau);
        m_sema3.tauReduceSet(tau);
    }

    auto update(Time time, Value sample) -> void
    {
        m_dema.update(time, sample);
        m_sema3.update(time, sema2());
    }

    auto value() const -> Value
    {
        return dema() + sema() - 2 * sema2() + sema3();
    }

    auto sema() const -> Value
    {
        return m_dema.sema();
    }

    auto sema2() const -> Value
    {
        return m_dema.sema2();
    }

    auto sema3() const -> Value
    {
        return m_sema3.value();
    }

    auto dema() const -> Value
    {
        return m_dema.value();
    }

    auto initialized() const -> bool
    {
        return m_sema3.initialized();
    }
};

using EMA = BasicEMA<double, double>;
using SingleEMA = BasicSingleEMA<double, double, double>;
using DoubleEMA = BasicDoubleEMA<double, double, double>;
using TripleEMA = BasicTripleEMA<double, double, double>;

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMA_HPP_
