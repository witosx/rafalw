#ifndef XTRAS_UTILS_INTERFACE_HPP_
#define XTRAS_UTILS_INTERFACE_HPP_

#include <xtras/templates.hpp>
#include <new>
#include <type_traits>
#include <cstdint>

namespace xtras {
inline namespace utils {

namespace detail {

template<typename _Base, template<typename> class _Impl>
class Interface
{
public:
    using Base = _Base;

    template<typename T>
    using Impl = _Impl<T>;

    template<typename T>
    Interface(T&& object)
    {
        using ImplT = Impl2<std::remove_reference_t<T>>;

        static_assert(sizeof(m_buffer) == sizeof(ImplT), "interface size mismatch");

        new (m_buffer) ImplT{ std::addressof(object) };
    }

    auto base() const -> const Base&
    {
        return *static_cast<const Base*>(static_cast<const void*>(m_buffer));
    }

    auto base() -> Base&
    {
        return *static_cast<Base*>(static_cast<void*>(m_buffer));
    }

private:
    template<typename T>
    class Base2 : public Base
    {
    protected:
        T* m_object = nullptr;
    };

    template<typename T>
    class Base3 : public Base2<T>
    {
    protected:
        auto object() const -> const T&
        {
            return *Base2<T>::m_object;
        }

        auto object() -> T&
        {
            return *Base2<T>::m_object;
        }
    };

    template<typename T>
    class Impl2 final : public Impl<Base3<T>>
    {
    public:
        Impl2(T* object)
        {
            Base2<T>::m_object = object;
        }
    };

    std::uint8_t m_buffer[sizeof(Base2<void>)];
};

template<typename Derived>
struct InterfaceAccess
{
    decltype(auto) object() const
    {
        return static_cast<const Derived*>(this)->m_interface.base();
    }

    decltype(auto) object()
    {
        return static_cast<Derived*>(this)->m_interface.base();
    }
};

struct InterfaceSpecsBasesLast {};

template<typename Derived>
using InterfaceBaseAccess = InterfaceAccess<Derived>;

template<typename Derived, typename Spec>
using InterfaceBase = typename Spec::template Impl<InterfaceBaseAccess<Derived>>;

template<typename... Specs>
struct InterfaceCompoundSpec
{
    template<typename T>
    using Base = typename templates::chain<Specs::template Base...>::template base<T>::template args<>;

    template<typename T>
    using Impl = typename templates::chain<Specs::template Impl...>::template base<T>::template args<>;
};

} // namespace detail

template<typename _Spec>
class InterfaceSimple : public detail::InterfaceBase<InterfaceSimple<_Spec>, _Spec>
{
public:
    using Spec = _Spec;

    template<typename T>
    InterfaceSimple(T&& object) :
        m_interface{ std::forward<T>(object) }
    {}

private:
    friend detail::InterfaceBaseAccess<InterfaceSimple<Spec>>;

    using Base = typename Spec::template Base<detail::InterfaceSpecsBasesLast>;

    template<typename T>
    using Impl = typename Spec::template Impl<T>;

    detail::Interface<Base, Impl> m_interface;
};

template<typename... Interfaces>
using InterfaceCompound = InterfaceSimple<detail::InterfaceCompoundSpec<typename Interfaces::Spec...>>;

} // namespace utils
} // namespace xtras

#endif // XTRAS_UTILS_INTERFACE_HPP_
