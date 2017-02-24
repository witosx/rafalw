#ifndef RAFALW_NUMERIC_EMA2_HPP_
#define RAFALW_NUMERIC_EMA2_HPP_

#include <rafalw/numeric/EMA1.hpp>

inline namespace rafalw {
namespace numeric {

template<typename SampleT, typename DurationT = double>
class EMA2
{
public:
    using SEMA = EMA1<SampleT, DurationT>;

    using Sample = typename SEMA::Sample;
    using Duration = typename SEMA::Duration;
    using Value = typename SEMA::Value;

    EMA2(Duration tau_grow, Duration tau_reduce) :
        m_sema{ tau_grow, tau_reduce },
        m_sema2{ tau_grow, tau_reduce }
    {}

    EMA2(Duration tau) :
        m_sema{ tau },
        m_sema2{ tau }
    {}

    auto initialize(Value sample) -> void
    {
        m_sema.initialize(sample);
        m_sema2.initialize(sample);
        m_value = sample;
    }

    auto initialized() const -> bool
    {
        return static_cast<bool>(m_value);
    }

    auto updateTau(Duration tau) -> void
    {
        m_sema.updateTau(tau, tau);
        m_sema2.updateTau(tau, tau);
    }

    auto updateTau(Duration tau_grow, Duration tau_reduce) -> void
    {
        m_sema.updateTau(tau_grow, tau_reduce);
        m_sema2.updateTau(tau_grow, tau_reduce);
    }

    auto update(Sample sample, Duration duration) -> void
    {
        m_sema.update(sample, duration);
        m_sema2.update(m_sema.value(), duration);
        m_value = 2 * m_sema.value() - m_sema2.value();
    }

    auto value() const -> Value
    {
        rafalw_utils_assert(m_value);
        return *m_value;
    }

    auto sema() const -> Value
    {
        return m_sema.sema();
    }

    auto sema2() const -> Value
    {
        return m_sema2.sema();
    }

    auto dema() const -> Value
    {
        return value();
    }

private:
    SEMA m_sema;
    SEMA m_sema2;
    boost::optional<Value> m_value;
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMA2_HPP_
