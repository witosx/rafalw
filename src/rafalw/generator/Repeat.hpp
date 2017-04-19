#ifndef RAFALW_GENERATOR_REPEAT_HPP_
#define RAFALW_GENERATOR_REPEAT_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename ValueT>
class Repeat : private Base
{
public:
    using Value = ValueT;

    Repeat(const Value& value) :
        m_value{ value }
    {}

private:
    friend class BaseAccess;

    Value m_value;

    auto generatorDone() const -> bool
    {
        return false;
    }

    decltype(auto) generatorPeek() const
    {
        return m_value;
    }

    auto generatorUpdate() -> void {}
};

template<typename Value>
auto repeat(const Value& value) -> Repeat<Value>
{
    return Repeat<Value>{ value };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_REPEAT_HPP_
