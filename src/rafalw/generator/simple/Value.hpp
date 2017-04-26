#ifndef RAFALW_GENERATOR_REPEAT_HPP_
#define RAFALW_GENERATOR_REPEAT_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename ValueT>
class Value : private Base
{
public:
    using Element = ValueT;

    explicit Value(const Element& value) :
        m_value{ value }
    {}

private:
    friend class BaseAccess;

    Element m_value;

    auto generatorDone() const -> bool
    {
        return false;
    }

    auto generatorPeek() const -> const Element&
    {
        return m_value;
    }

    auto generatorUpdate() -> void
    {
        // nothing to do
    }

    auto generatorReset()
    {
        return RESET_TAG<true>;
    }
};

template<typename ValueT>
auto value(const ValueT& value) -> Value<ValueT>
{
    return Value<ValueT>{ value };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_REPEAT_HPP_
