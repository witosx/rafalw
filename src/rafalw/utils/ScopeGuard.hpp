#ifndef RAFALW_UTILS_SCOPEGUARD_HPP_
#define RAFALW_UTILS_SCOPEGUARD_HPP_

#include <type_traits>
#include <utility>

inline namespace rafalw {
namespace utils {

template<typename CallbackT>
class ScopeGuard
{
public:
    using Callback = CallbackT;

    ScopeGuard(Callback func) :
        m_callback{ std::move(func) },
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

private:
    Callback m_callback;
    bool m_active;

    auto fire() -> void
    {
        if (m_active)
        {
            m_callback();
            m_active = false;
        }
    }
};

template<typename F>
auto scope_guard(F&& func) -> ScopeGuard<std::remove_reference_t<F>>
{
    return ScopeGuard<std::remove_reference_t<F>>{ std::forward<F>(func) };
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_SCOPEGUARD_HPP_
