#ifndef RAFALW_GENERATOR_SIMPLE_FUNCTION_HPP_
#define RAFALW_GENERATOR_SIMPLE_FUNCTION_HPP_

#include <rafalw/generator/Base.hpp>
#include <rafalw/generator/detail.hpp>

inline namespace rafalw {
namespace generator {

template<typename FuncT>
class Function : private Base
{
public:
    template<typename F, std::enable_if_t<!std::is_same<std::decay_t<F>, Function<FuncT>>::value>* = nullptr>
    Function(F&& f) :
        m_funcOrig{ std::forward<F>(f) }
    {
        generatorUpdate();
    }

private:
    friend class BaseAccess;

    using Func = FuncT;

    Func m_funcOrig;
    Func m_func = m_funcOrig;

    using Value = decltype(m_func());

    detail::ResultStore<Value> m_value;

    auto generatorDone() const -> bool
    {
        return false;
    }

    decltype(auto) generatorPeek() const
    {
        return m_value.get();
    }

    auto generatorUpdate() -> void
    {
        m_value.set(m_func());
    }

    auto generatorReset()
    {
        m_func.~Func();
        new (&m_func) Func{ m_funcOrig };

        generatorUpdate();
        return RESET_OK;
    }
};

template<typename Func>
auto function(Func&& func) -> Function<std::decay_t<Func>>
{
    return Function<std::decay_t<Func>>{ std::forward<Func>(func) };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_SIMPLE_FUNCTION_HPP_

