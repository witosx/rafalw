#ifndef RAFALW_NUMERIC_EMABASE_HPP_
#define RAFALW_NUMERIC_EMABASE_HPP_

#include <rafalw/utils/assert.hpp>
#include <boost/optional.hpp>
#include <cmath>
#include <utility>

inline namespace rafalw {
namespace numeric {

template<typename SampleT = double, typename AlphaT = double>
class EMABase
{
public:
    using Sample = SampleT;
    using Alpha = AlphaT;
    using Value = decltype(std::declval<Sample>() * std::declval<Alpha>());

    EMABase() = default;

    EMABase(Value v) :
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

} // namespace numeric
} // namespace rafalw

#endif // RAFALW_NUMERIC_EMABASE_HPP_
