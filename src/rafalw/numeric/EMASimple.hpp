#ifndef RAFALW_NUMERIC_EMASIMPLE_HPP_
#define RAFALW_NUMERIC_EMASIMPLE_HPP_

#include <rafalw/numeric/EMABase.hpp>
#include <boost/optional.hpp>
#include <cmath>
#include <utility>

inline namespace rafalw {
namespace numeric {

template<typename SampleT = double, typename AlphaT = double>
class EMASimple
{
public:
    using Sample = SampleT;
    using Alpha = AlphaT;
    using UnderlyingEMA = EMABase<Sample, Alpha>;
    using Value = typename UnderlyingEMA::Value;

    EMASimple(Alpha alpha_grow, Alpha alpha_reduce) :
        m_alphaGrow{ alpha_grow },
        m_alphaReduce{ alpha_reduce }
    {}

    EMASimple(Alpha alpha) :
        EMASimple{ alpha, alpha }
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

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMASIMPLE_HPP_
