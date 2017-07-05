#ifndef XTRAS_GENERATOR_SIMPLE_FUNCTION_HPP_
#define XTRAS_GENERATOR_SIMPLE_FUNCTION_HPP_

#include <xtras/generator/Base.hpp>
#include <xtras/generator/detail.hpp>

namespace xtras {
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
        return RESET_TAG<true>;
    }
};

template<typename Func>
auto function(Func&& func) -> Function<std::decay_t<Func>>
{
    return Function<std::decay_t<Func>>{ std::forward<Func>(func) };
}

} // namespace generator
} // namespace xtras

#endif // XTRAS_GENERATOR_SIMPLE_FUNCTION_HPP_

