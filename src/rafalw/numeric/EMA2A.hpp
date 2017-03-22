#ifndef RAFALW_NUMERIC_EMA2A_HPP_
#define RAFALW_NUMERIC_EMA2A_HPP_

#include <rafalw/numeric/EMA1.hpp>

inline namespace rafalw {
namespace numeric {

template<typename SampleT, typename DurationT>
class EMA2A
{
public:
    using EMA1 = EMA1<SampleT, DurationT>;

    using Sample = typename EMA1::Sample;
    using Duration = typename EMA1::Duration;
    using Value = typename EMA1::Value;

    template<typename, typename ValueT>
    struct Param
    {
        ValueT value;
    };

    using Tau1 = Param<class ParamTau1, Duration>;
    using Tau2 = Param<class ParamTau2, Duration>;

    template<typename... ParamTagT, typename... ParamTypeT>
    EMA2A(const Param<ParamTagT, ParamTypeT>... params) :
        EMA2A{ std::make_tuple(params...) }
    {}

    auto initialized() const -> bool
    {
        return static_cast<bool>(m_value);
    }

    auto initialize(const Value sample) -> void
    {
        m_ema1.initialize(sample);
        m_ema2.initialize(sample);
        m_value = sample;
    }

    auto updateParam(const Tau1 tau) -> void
    {
        m_ema1.updateParam(typename EMA1::Tau{ tau.value });
    }

    auto updateParam(const Tau2 tau) -> void
    {
        m_ema2.updateParam(typename EMA1::Tau{ tau.value });
    }

    auto update(Sample sample, Duration duration) -> void
    {
        m_ema1.update(sample, duration);
        m_ema2.update(m_ema1.value(), duration);
        m_value = 2 * m_ema1.value() - m_ema2.value();
    }

    auto value() const -> Value
    {
        rafalw_utils_assert(m_value);
        return *m_value;
    }

    auto ema1() const -> Value
    {
        return m_ema1.value();
    }

    auto ema2() const -> Value
    {
        return m_ema2.value();
    }

private:
    EMA1 m_ema1;
    EMA1 m_ema2;
    boost::optional<Value> m_value;

    template<typename... Params>
    EMA2A(const std::tuple<Params...>& params) :
        m_ema1{ typename EMA1::Tau{ std::get<Tau1>(params).value } },
        m_ema2{ typename EMA1::Tau{ std::get<Tau2>(params).value } }
    {}
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMA2A_HPP_
