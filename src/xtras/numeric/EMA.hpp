#ifndef XTRAS_NUMERIC_EMA_HPP_
#define XTRAS_NUMERIC_EMA_HPP_

#include <xtras/numeric/EMA1.hpp>
#include <xtras/numeric/EMA2A.hpp>
#include <xtras/numeric/EMA2B.hpp>
#include <boost/variant.hpp>

namespace xtras {
namespace numeric {

template<typename SampleT, typename DurationT>
class EMA
{
public:
    using Sample = SampleT;
    using Duration = DurationT;

    using EMA1 = numeric::EMA1<Sample, Duration>;
    using EMA2A = numeric::EMA2A<Sample, Duration>;
    using EMA2B = numeric::EMA2B<Sample, Duration>;

    using Value = typename EMA1::Value;

    template<typename EMAT>
    EMA(EMAT ema) :
        m_variant{ std::move(ema) }
    {}

    auto initialized() const -> bool
    {
        return apply_visitor([=](const auto& ema){
            return ema.initialized();
        }, m_variant);
    }

    auto initialize(const Value sample) -> void
    {
        apply_visitor([=](auto& ema){
            ema.initialize(sample);
        }, m_variant);
    }

    auto value() const -> Value
    {
        return apply_visitor([=](const auto& ema){
            return ema.value();
        }, m_variant);
    }

    auto update(const Sample sample, const Duration duration) -> void
    {
        apply_visitor([=](auto& ema){
            ema.update(sample, duration);
        }, m_variant);
    }

private:
    boost::variant<EMA1, EMA2A, EMA2B> m_variant;
};

} // namespace numeric
} // namespace xtras

#endif // XTRAS_NUMERIC_EMA_HPP_
