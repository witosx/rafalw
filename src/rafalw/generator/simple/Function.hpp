#ifndef RAFALW_GENERATOR_SIMPLE_FUNCTION_HPP_
#define RAFALW_GENERATOR_SIMPLE_FUNCTION_HPP_

#include <rafalw/generator/Base.hpp>
#include <boost/optional.hpp>

inline namespace rafalw {
namespace generator {

namespace detail {

template<typename T>
class ResultStore;

} // namespace detail

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

namespace detail {

template<typename T>
class ResultStore
{
public:
    auto set(T&& v) -> void
    {
        m_data.emplace(std::move(v));
    }

    auto get() const -> const T&
    {
        return *m_data;
    }

private:
    boost::optional<T> m_data;
};

template<typename T>
class ResultStore<T&>
{
public:
    auto set(T& v) -> void
    {
        m_data = &v;
    }

    auto get() const -> T&
    {
        return *m_data;
    }

private:
    T* m_data{ nullptr };
};

} // namespace detail

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_SIMPLE_FUNCTION_HPP_

