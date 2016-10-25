#ifndef RAFALW_UTILS_INTERFACEVALUE_HPP_
#define RAFALW_UTILS_INTERFACEVALUE_HPP_

#include <rafalw/utils/Interface.hpp>
#include <rafalw/utils/ValueHolder.hpp>

inline namespace rafalw {
namespace utils {

template<typename T>
constexpr auto InterfaceValueEmplace = ValueHolderEmplace<T>;

template<typename _Interface>
class InterfaceValue : public detail::InterfaceBase<InterfaceValue<_Interface>, typename _Interface::Spec>
{
public:
    using Interface = _Interface;

    template<typename T>
    InterfaceValue(T&& object) :
        m_holder{ std::forward<T>(object) },
        m_interface{ *static_cast<T*>(m_holder.pointer()) }
    {}

    template<typename T, typename... Args>
    InterfaceValue(ValueHolderEmplaceTag<T> tag, Args&&... args) :
        m_holder{ tag, std::forward<Args>(args)... },
        m_interface{ *static_cast<T*>(m_holder.pointer()) }
    {}

private:
    friend detail::InterfaceBaseAccess<InterfaceValue<_Interface>>;

    class InterfaceWrapper
    {
    public:
        template<typename T>
        InterfaceWrapper(T&& o) :
            m_interface{ std::forward<T>(o) }
        {}

        auto base() const -> const Interface&
        {
            return m_interface;
        }

        auto base() -> Interface&
        {
            return m_interface;
        }

    private:
        Interface m_interface;
    };

    ValueHolder m_holder;
    InterfaceWrapper m_interface;
};

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_INTERFACEVALUE_HPP_
