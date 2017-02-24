#ifndef RAFALW_NUMERIC_EMA_HPP_
#define RAFALW_NUMERIC_EMA_HPP_

#include <rafalw/numeric/EMA1.hpp>
#include <rafalw/numeric/EMA2.hpp>
#include <rafalw/numeric/EMA3.hpp>
#include <boost/variant.hpp>

inline namespace rafalw {
namespace numeric {

enum class EMAType
{
    SINGLE,
    DOUBLE,
    TRIPLE
};

template<typename SampleT, typename DurationT = double>
class EMA
{
private:
    using SEMA = EMA1<SampleT, DurationT>;
    using DEMA = EMA2<SampleT, DurationT>;
    using TEMA = EMA3<SampleT, DurationT>;
    using XEMA = boost::variant<SEMA, DEMA, TEMA>;

public:
    using Sample = typename SEMA::Sample;
    using Duration = typename SEMA::Duration;
    using Value = typename SEMA::Value;

    EMA(EMAType type, Duration tau) :
        m_ema{ create(type, tau) }
    {}

    EMA(EMAType type, Duration tau_grow, Duration tau_reduce) :
        m_ema{ create(type, tau_grow, tau_reduce) }
    {}

    auto initialized() const -> bool
    {
        return boost::apply_visitor([this](const auto& o){
            return o.initialized();
        }, m_ema);
    }

    auto initialize(Value sample) -> void
    {
        boost::apply_visitor([this,sample](auto& o){
            return o.initialize(sample);
        }, m_ema);
    }

    auto value() const -> Value
    {
        return boost::apply_visitor([this](const auto& o){
            return o.value();
        }, m_ema);
    }

    auto updateTau(Duration tau) -> void
    {
        boost::apply_visitor([this,tau](auto& o){
            return o.updateTau(tau);
        }, m_ema);
    }

    auto updateTau(Duration tau_grow, Duration tau_reduce) -> void
    {
        boost::apply_visitor([this,tau_grow,tau_reduce](auto& o){
            return o.updateTau(tau_grow, tau_reduce);
        }, m_ema);
    }

    auto update(Sample sample, Duration duration) -> void
    {
        boost::apply_visitor([this,sample,duration](auto& o){
            return o.update(sample,duration);
        }, m_ema);
    }

private:
    static auto create(EMAType type, Duration tau_grow, Duration tau_reduce) -> XEMA
    {
        switch (type) {
        case EMAType::SINGLE:
            return SEMA{ tau_grow, tau_reduce };
        case EMAType::DOUBLE:
            return DEMA{ tau_grow, tau_reduce };
        case EMAType::TRIPLE:
            return TEMA{ tau_grow, tau_reduce };
        }
    }

    XEMA m_ema;
};

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMA_HPP_
