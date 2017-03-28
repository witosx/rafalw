#ifndef RAFALW_NUMERIC_EMA2B_HPP_
#define RAFALW_NUMERIC_EMA2B_HPP_

#include <rafalw/numeric/EMA1.hpp>

inline namespace rafalw {
namespace numeric {

template<typename SampleT, typename DurationT>
class EMA2B
{
public:
    using Sample = SampleT;
    using Duration = DurationT;

    using Alpha = decltype(std::exp(-std::declval<Duration>() / std::declval<Duration>()));
    using Value = decltype(std::declval<Sample>() * std::declval<Alpha>());

    using TrendEMA = EMA1<Sample, Duration>;

    template<typename, typename ValueT>
    struct Param
    {
        ValueT value;
    };

    using Tau = Param<class ParamTau, Duration>;
    using TrendTau = Param<class ParamTrendTau, Duration>;

    template<typename... ParamTagT, typename... ParamTypeT>
    EMA2B(const Param<ParamTagT, ParamTypeT>... params) :
        EMA2B{ std::make_tuple(params...) }
    {}

    auto initialize(const Value sample) -> void
    {
        m_value = sample;
        m_trend.initialize(Value{ 0 });
    }

    auto initialized() const -> bool
    {
        return static_cast<bool>(m_value);
    }

    auto updateParam(const Tau tau) -> void
    {
        m_tau = tau.value;
    }

    auto updateParam(const TrendTau tau) -> void
    {
        m_trend.updateParam(typename TrendEMA::Tau{ tau.value });
    }

    auto update(const Sample sample, const Duration duration) -> void
    {
        const auto prev_value = m_value;
        m_value = calculateValue(sample, duration);

        if (prev_value)
            m_trend.update(*m_value - *prev_value, duration);
    }

    auto value() const -> Value
    {
        rafalw_utils_assert(m_value);
        return *m_value;
    }

    auto trend() const -> Value
    {
        return m_trend.value();
    }

private:
    Duration m_tau;
    TrendEMA m_trend;
    boost::optional<Value> m_value;

    template<typename... Params>
    EMA2B(const std::tuple<Params...>& params) :
        m_tau{ std::get<Tau>(params).value },
        m_trend{ typename TrendEMA::Tau{ std::get<TrendTau>(params).value } }
    {}

    auto calculateValue(const Sample sample, const Duration duration) const -> Value
    {
        if (!m_value)
            return sample;

        const auto alpha = std::exp(-duration / m_tau);
        return calculateValueFromAlpha(sample, alpha);
    }

    auto calculateValueFromAlpha(const Sample sample, const Alpha alpha) const -> Value
    {
        rafalw_utils_assert(alpha >= Alpha{ 0 });
        rafalw_utils_assert(alpha <= Alpha{ 1 });
        return alpha * (value() + trend()) + (Alpha{ 1 } - alpha) * sample;
    }
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMA2B_HPP_
