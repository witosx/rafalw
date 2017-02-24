#ifndef RAFALW_NUMERIC_EMA1_HPP_
#define RAFALW_NUMERIC_EMA1_HPP_

#include <rafalw/utils/assert.hpp>
#include <boost/optional.hpp>
#include <cmath>
#include <utility>

inline namespace rafalw {
namespace numeric {

template<typename SampleT, typename DurationT = double>
class EMA1
{
public:
    using Sample = SampleT;
    using Duration = DurationT;

    using Alpha = decltype(std::exp(-std::declval<Duration>() / std::declval<Duration>()));
    using Value = decltype(std::declval<Sample>() * std::declval<Alpha>());

    EMA1(Duration tau_grow, Duration tau_reduce) :
        m_tauGrow{ tau_grow },
        m_tauReduce{ tau_reduce }
    {}

    EMA1(Duration tau) :
        m_tauGrow{ tau, tau },
        m_tauReduce{ tau, tau }
    {}

    auto initialized() const -> bool
    {
        return static_cast<bool>(m_value);
    }

    auto initialize(Value sample) -> void
    {
        m_value = sample;
    }

    auto value() const -> Value
    {
        rafalw_utils_assert(m_value);
        return *m_value;
    }

    auto sema() const -> Value
    {
        return value();
    }

    auto updateTau(Duration tau) -> void
    {
        updateTau(tau, tau);
    }

    auto updateTau(Duration tau_grow, Duration tau_reduce) -> void
    {
        m_tauGrow = tau_grow;
        m_tauReduce = tau_reduce;
    }

    auto update(Sample sample, Duration duration) -> void
    {
        m_value = calculateValue(sample, duration);
    }

private:
    Duration m_tauGrow;
    Duration m_tauReduce;

    boost::optional<Value> m_value;

    auto calculateValue(const Sample sample, const Duration duration) const -> Value
    {
        if (!m_value)
            return sample;

        const auto tau = (sample < value() ? m_tauReduce : m_tauGrow);
        const auto alpha = std::exp(-duration / tau);
        return calculateValueFromAlpha(sample, alpha);
    }

    auto calculateValueFromAlpha(const Sample sample, const Alpha alpha) const -> Value
    {
        rafalw_utils_assert(alpha >= Alpha{ 0 });
        rafalw_utils_assert(alpha <= Alpha{ 1 });
        rafalw_utils_assert(m_value);

        return (alpha * value()) + sample - alpha * sample;;
    }
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMA1_HPP_
