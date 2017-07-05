#ifndef RAFALW_ENUMS_HPP_
#define RAFALW_ENUMS_HPP_

#include <type_traits>
#include <functional>

inline namespace rafalw {
namespace enums {

    template<typename T>
    struct EnableBitwiseOperatorsFor;

    namespace detail {

        template<typename T, typename = std::void_t<>>
        struct Enabled : public std::false_type {};

        template<typename T>
        struct Enabled<T, std::void_t<decltype(EnableBitwiseOperatorsFor<T>{})>> : public std::true_type {};

        template<typename T>
        using enabler = std::enable_if_t<std::is_enum_v<T> && Enabled<T>::value>*;

        template<typename OpT, typename EnumT>
        constexpr auto op(EnumT e1, EnumT e2) noexcept -> EnumT
        {
            using IntT = std::underlying_type_t<EnumT>;
            return static_cast<EnumT>(OpT{}(static_cast<IntT>(e1), static_cast<IntT>(e2)));
        }

        template<typename OpT, typename EnumT>
        constexpr auto op(EnumT e1) noexcept -> EnumT
        {
            using IntT = std::underlying_type_t<EnumT>;
            return static_cast<EnumT>(OpT{}(static_cast<IntT>(e1)));
        }

    } // namespace detail

} // namespace enums
} // namespace rafalw

template<typename EnumT, rafalw::enums::detail::enabler<EnumT> = nullptr>
constexpr auto operator |(EnumT v1, EnumT v2) noexcept -> EnumT
{
    return rafalw::enums::detail::op<std::bit_or<>>(v1, v2);
}

template<typename EnumT, rafalw::enums::detail::enabler<EnumT> = nullptr>
constexpr auto operator &(EnumT v1, EnumT v2) noexcept -> EnumT
{
    return rafalw::enums::detail::op<std::bit_and<>>(v1, v2);
}

template<typename EnumT, rafalw::enums::detail::enabler<EnumT> = nullptr>
constexpr auto operator ^(EnumT v1, EnumT v2) noexcept -> EnumT
{
    return rafalw::enums::detail::op<std::bit_xor<>>(v1, v2);
}

template<typename EnumT, rafalw::enums::detail::enabler<EnumT> = nullptr>
constexpr auto operator ~(EnumT v1) noexcept -> EnumT
{
    return rafalw::enums::detail::op<std::bit_not<>>(v1);
}

#endif // RAFALW_ENUMS_HPP_
