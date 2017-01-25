#ifndef RAFALW_NUMERIC_EMA_HPP_
#define RAFALW_NUMERIC_EMA_HPP_

#include <rafalw/utils/assert.hpp>
#include <boost/optional.hpp>
#include <cmath>
#include <utility>

inline namespace rafalw {
namespace numeric {

template<typename SampleT = double, typename AlphaT = double>
class BasicEMA
{
public:
    using Sample = SampleT;
    using Alpha = AlphaT;
    using Value = decltype(std::declval<Sample>() * std::declval<Alpha>());

    BasicEMA() = default;

    BasicEMA(Value v) :
        m_value{ v }
    {}

    auto initialized() const -> bool
    {
        return static_cast<bool>(m_value);
    }

    auto initialize(Value value) -> void
    {
        m_value = value;
    }

    auto value() const -> Value
    {
        rafalw_utils_assert(initialized());
        return *m_value;
    }

    auto valueOr(Value vdef) const -> Value
    {
        return m_value.value_or(vdef);
    }

    auto update(Sample sample, Alpha alpha) -> void
    {
        if (!initialized())
            m_value = sample;
        else
            m_value = calculateValue(sample, alpha);
    }

private:
    boost::optional<Value> m_value;

    auto calculateValue(Sample sample, Alpha alpha) const -> Value
    {
        rafalw_utils_assert(alpha >= 0 && alpha <= 1);
        return (alpha * value()) + sample - alpha * sample;;
    }
};

template<typename SampleT = double, typename AlphaT = double>
class RegularEMA
{
public:
    using Sample = SampleT;
    using Alpha = AlphaT;
    using UnderlyingEMA = BasicEMA<Sample, Alpha>;
    using Value = typename UnderlyingEMA::Value;

    RegularEMA(Alpha alpha_grow, Alpha alpha_reduce) :
        m_alphaGrow{ alpha_grow },
        m_alphaReduce{ alpha_reduce }
    {}

    RegularEMA(Alpha alpha) :
        RegularEMA{ alpha, alpha }
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

    auto valueOr(Value vdef) const -> Value
    {
        return m_ema.valueOr(vdef);
    }

    auto update(Sample sample) -> void
    {
        if (!initialized())
        {
            initialize(sample);
            return;
        }

        const auto alpha = (sample > value() ? m_alphaGrow : m_alphaReduce);

        m_ema.update(sample, alpha);
    }

private:
    Alpha m_alphaGrow;
    Alpha m_alphaReduce;
    UnderlyingEMA m_ema;
};

template<typename SampleT, typename DurationT = double>
class DurationBasedEMA
{
public:
    using Duration = DurationT;
    using Sample = SampleT;
    using Alpha = decltype(std::exp(std::declval<Duration>() / std::declval<Duration>()));

    using UnderlyingEMA = BasicEMA<Sample, Alpha>;
    using Value = typename UnderlyingEMA::Value;

    DurationBasedEMA(Duration tau_grow, Duration tau_reduce) :
        m_tauGrow{ tau_grow },
        m_tauReduce{ tau_reduce }
    {}

    DurationBasedEMA(Duration tau) :
        DurationBasedEMA{ tau, tau }
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

    auto valueOr(Value vdef) const -> Value
    {
        return m_ema.valueOr(vdef);
    }

    auto update(Sample sample, Duration duration) -> void
    {
        const auto tau = (sample > value() ? m_tauGrow : m_tauReduce);
        const auto alpha = std::exp(-duration / tau);
        m_ema.update(sample, alpha);
    }

private:
    Duration m_tauGrow;
    Duration m_tauReduce;
    UnderlyingEMA m_ema;
};

template<typename SampleT, typename TimeT = double>
class TimeBasedEMA
{
public:
    using Sample = SampleT;
    using Time = TimeT;
    using Duration = decltype(std::declval<Time>() - std::declval<Time>());
    using UnderlyingEMA = DurationBasedEMA<Sample, Duration>;
    using Value = typename UnderlyingEMA::Value;

    TimeBasedEMA(Duration tau_grow, Duration tau_reduce) :
        m_ema{ tau_grow, tau_reduce }
    {}

    TimeBasedEMA(Duration tau) :
        m_ema{ tau }
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

    auto valueOr(Value vdef) const -> Value
    {
        return m_ema.valueOr(vdef);
    }

    auto update(Time time, Sample sample) -> void
    {
        update(time);
        m_data = Data{ time, sample };
    }

    auto update(Time time) -> void
    {
        if (!m_data)
            return;

        rafalw_utils_assert(time >= m_data->m_time);
        m_ema.update(m_data->sample, time - m_data->time);
        m_data->time = time;
    }

private:
    UnderlyingEMA m_ema;

    struct Data
    {
        Time time;
        Sample sample;
    };

    boost::optional<Data> m_data;
};

//template<typename SampleT, typename TimeT>
//class EMA
//{
//public:
//    using Time = TimeT;
//    using Sample = SampleT;
//
//    using TimeDelta = decltype(std::declval<Time>() - std::declval<Time>());
//    using Alpha = decltype(std::exp(std::declval<TimeDelta>() / std::declval<TimeDelta>()));
//
//    using BasicEMA = BasicEMA<Sample, Alpha>;
//    using Value = typename BasicEMA::Value;
//
//    EMA(TimeDelta tau_grow, TimeDelta tau_reduce) :
//        m_tauGrow{ tau_grow },
//        m_tauReduce{ tau_reduce }
//    {}
//
//    EMA(TimeDelta tau) :
//        EMA{ tau, tau }
//    {}
//
//    auto initialize(Time time, Value sample) -> void
//    {
//        m_time = time;
//        m_ema.initialize(sample);
//    }
//
//    auto update(Time time, Sample sample) -> void
//    {
//        if (!initialized())
//        {
//            initialize(time, sample);
//            return;
//        }
//
//        rafalw_utils_assert(time > m_time);
//
//        const auto dt = time - m_time;
//        const auto tau = (sample > value() ? m_tauGrow : m_tauReduce);
//        const auto alpha = std::exp(-dt / tau);
//
//        m_ema.update(alpha, sample);
//        m_time = time;
//    }
//
//    auto value() const -> Value
//    {
//        return m_ema.value();
//    }
//
//    auto valueOr(Value vdef) const -> Value
//    {
//        return m_ema.valueOr(vdef);
//    }
//
//    auto initialized() const -> bool
//    {
//        return m_ema.initialized();
//    }
//
//private:
//    TimeDelta m_tauGrow;
//    TimeDelta m_tauReduce;
//
//    BasicEMA m_ema;
//    Time m_time;
//};

//template<typename EmaT>
//class EMAPostWrapper
//{
//public:
//    using EMA = EmaT;
//
//    using Time = typename EMA::Time;
//    using Sample = typename EMA::Sample;
//    using Value = typename EMA::Value;
//
//    template<typename... Args>
//    EMAPostWrapper(Args&&... args) :
//        m_ema{ std::forward<Args>(args) }
//    {}
//
//    auto initialize(Time time, Value value) -> void
//    {
//        m_ema.initialize(time, value);
//        m_sample = value;
//    }
//
//    auto update(Time time, Sample sample) -> void
//    {
//        if (!initialized())
//        {
//            initialize(time, sample);
//            return;
//        }
//
//        m_ema.update(time, m_sample);
//        m_sample = sample;
//    }
//
//    auto value() const -> Value
//    {
//        return m_ema.value();
//    }
//
//    auto valueOr(Value vdef) const -> Value
//    {
//        return m_ema.valueOr(vdef);
//    }
//
//    auto initialized() const -> bool
//    {
//        return m_ema.initialized();
//    }
//
//private:
//    EMA m_ema;
//    Sample m_sample;
//};

//template<typename _Time, typename _Alpha, typename _Value>
//class BasicDoubleEMA
//{
//private:
//    using SEMA = BasicSingleEMA<_Time, _Alpha, _Value>;
//
//public:
//    using Time = typename SEMA::Time;
//    using TimeDelta = typename SEMA::TimeDelta;
//    using Value = typename SEMA::Value;
//
//private:
//    SEMA m_sema;
//    SEMA m_sema2;
//
//public:
//    BasicDoubleEMA(EMAMode mode) :
//        m_sema{ mode },
//        m_sema2{ EMAMode::POST }
//    {}
//
//    auto tauSet(TimeDelta tau) -> void
//    {
//        m_sema.tauGrowSet(tau);
//        m_sema.tauReduceSet(tau);
//
//        m_sema2.tauGrowSet(tau);
//        m_sema2.tauReduceSet(tau);
//    }
//
//    auto update(Time time, Value sample) -> void
//    {
//        m_sema.update(time, sample);
//        m_sema2.update(time, sema());
//    }
//
//    auto value() const -> Value
//    {
//        return 2 * sema() - sema2();
//    }
//
//    auto sema() const -> Value
//    {
//        return m_sema.value();
//    }
//
//    auto sema2() const -> Value
//    {
//        return m_sema2.value();
//    }
//
//    auto initialized() const -> bool
//    {
//        return m_sema.initialized();
//    }
//};
//
//template<typename _Time, typename _Alpha, typename _Value>
//class BasicTripleEMA
//{
//private:
//    using SEMA = BasicSingleEMA<_Time, _Alpha, _Value>;
//    using DEMA = BasicDoubleEMA<_Time, _Alpha, _Value>;
//
//public:
//    using Time = typename SEMA::Time;
//    using TimeDelta = typename SEMA::TimeDelta;
//    using Value = typename SEMA::Value;
//
//private:
//    DEMA m_dema;
//    SEMA m_sema3;
//
//public:
//    BasicTripleEMA(EMAMode mode) :
//        m_dema{ mode },
//        m_sema3{ EMAMode::POST }
//    {}
//
//    auto tauSet(TimeDelta tau) -> void
//    {
//        m_dema.tauGrowSet(tau);
//        m_dema.tauReduceSet(tau);
//
//        m_sema3.tauGrowSet(tau);
//        m_sema3.tauReduceSet(tau);
//    }
//
//    auto update(Time time, Value sample) -> void
//    {
//        m_dema.update(time, sample);
//        m_sema3.update(time, sema2());
//    }
//
//    auto value() const -> Value
//    {
//        return dema() + sema() - 2 * sema2() + sema3();
//    }
//
//    auto sema() const -> Value
//    {
//        return m_dema.sema();
//    }
//
//    auto sema2() const -> Value
//    {
//        return m_dema.sema2();
//    }
//
//    auto sema3() const -> Value
//    {
//        return m_sema3.value();
//    }
//
//    auto dema() const -> Value
//    {
//        return m_dema.value();
//    }
//
//    auto initialized() const -> bool
//    {
//        return m_sema3.initialized();
//    }
//};
//
//using EMA = BasicEMA<double, double>;
//using SingleEMA = BasicSingleEMA<double, double, double>;
//using DoubleEMA = BasicDoubleEMA<double, double, double>;
//using TripleEMA = BasicTripleEMA<double, double, double>;

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMA_HPP_
