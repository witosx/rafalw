#ifndef RAFALW_NUMERIC_EMA3_HPP_
#define RAFALW_NUMERIC_EMA3_HPP_

#include <rafalw/numeric/EMA1.hpp>
#include <rafalw/numeric/EMA2A.hpp>
#include <cmath>
#include <utility>

inline namespace rafalw {
namespace numeric {

template<typename SampleT, typename DurationT>
class EMA3
{
public:
    using Sample = SampleT;
    using Duration = DurationT;

    using EMA1 = EMA1<Sample, Duration>;
    using EMA2 = EMA2A<Sample, Duration>;

    using Value = typename EMA2::Value;

    EMA3(Duration tau_grow, Duration tau_reduce) :
        m_sema3{ tau_grow, tau_reduce },
        m_dema{ tau_grow, tau_reduce }
    {}

    EMA3(Duration tau) :
        m_sema3{ tau },
        m_dema{ tau }
    {}

    auto initialize(Value sample) -> void
    {
        m_sema3.initialize(sample);
        m_dema.initialize(sample);
        m_value = sample;
    }

    auto initialized() const -> bool
    {
        return static_cast<bool>(m_value);
    }

    auto updateTau(Duration tau) -> void
    {
        m_sema3.updateTau(tau, tau);
        m_dema.updateTau(tau, tau);
    }

    auto updateTau(Duration tau_grow, Duration tau_reduce) -> void
    {
        m_sema3.updateTau(tau_grow, tau_reduce);
        m_dema.updateTau(tau_grow, tau_reduce);
    }

    auto update(Sample sample, Duration duration) -> void
    {
        m_dema.update(sample, duration);
        m_sema3.update(sema2(), duration);
        m_value = dema() + sema() - 2 * sema2() + sema3();
    }

    auto value() const -> Value
    {
        rafalw_utils_assert(m_value);
        return *m_value;
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

    auto tema() const -> Value
    {
        return value();
    }

private:
    EMA1 m_sema3;
    EMA2 m_dema;

    boost::optional<Value> m_value;
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMA3_HPP_
