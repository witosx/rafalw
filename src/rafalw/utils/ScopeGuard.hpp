#ifndef RAFALW_UTILS_SCOPEGUARD_HPP_
#define RAFALW_UTILS_SCOPEGUARD_HPP_

#include <utility>

inline namespace rafalw {
namespace utils {

template<typename _Callback>
class ScopeGuard
{
public:
    using Callback = _Callback;

    ScopeGuard(Callback func) :
        m_callback{ func },
        m_active{ true }
    {}

    ScopeGuard(ScopeGuard&& o) :
        m_callback{ std::move(o.m_callback) },
        m_active{ std::exchange(o.m_active, false) }
    {}

    ScopeGuard(const ScopeGuard&) = delete;

    ~ScopeGuard()
    {
        fire();
    }

    auto fire() -> void
    {
        if (m_active)
        {
            m_callback();
            m_active = false;
        }
    }

    auto cancel() -> void
    {
        m_active = false;
    }

private:
    Callback m_callback;
    bool m_active;
};

template<typename F>
auto scope_guard(F func) -> ScopeGuard<F>
{
    return ScopeGuard<F>{ std::move(func) };
}

template<typename F, typename... Args>
auto scope_guard(F func, Args&&... args)
{
    return scope_guard([&,func]() mutable { func(std::forward<Args>(args)...); });
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_SCOPEGUARD_HPP_
