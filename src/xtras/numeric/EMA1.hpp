#ifndef XTRAS_NUMERIC_EMA1_HPP_
#define XTRAS_NUMERIC_EMA1_HPP_

#include <xtras/utils/assert.hpp>
#include <optional>
#include <cmath>
#include <utility>
#include <tuple>

namespace xtras {
namespace numeric {

template<typename SampleT, typename DurationT>
class EMA1
{
public:
    using Sample = SampleT;
    using Duration = DurationT;

    using Alpha = decltype(std::exp(-std::declval<Duration>() / std::declval<Duration>()));
    using Value = decltype(std::declval<Sample>() * std::declval<Alpha>());

    template<typename, typename ValueT>
    struct Param
    {
        ValueT value;
    };

    using Tau = Param<class ParamTau, Duration>;

    template<typename... ParamTagT, typename... ParamTypeT>
    EMA1(const Param<ParamTagT, ParamTypeT>... params) :
        EMA1{ std::make_tuple(params...) }
    {}

    auto initialized() const -> bool
    {
        return static_cast<bool>(m_value);
    }

    auto initialize(const Value sample) -> void
    {
        m_value = sample;
    }

    auto value() const -> Value
    {
        xtras_assert(m_value);
        return *m_value;
    }

    auto updateParam(const Tau tau) -> void
    {
        m_tau = tau.value;
    }

    auto update(const Sample sample, const Duration duration) -> void
    {
        m_value = calculateValue(sample, duration);
    }

private:
    Duration m_tau;
    std::optional<Value> m_value;

    template<typename... Params>
    EMA1(const std::tuple<Params...>& params) :
        m_tau{ std::get<Tau>(params).value }
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
        xtras_assert(alpha >= Alpha{ 0 });
        xtras_assert(alpha <= Alpha{ 1 });
        return alpha * value() + (Alpha{ 1 } - alpha) * sample;
    }
};

} // namespace numeric
} // namespace xtras

#endif // XTRAS_NUMERIC_EMA1_HPP_
