#ifndef XTRAS_ENUMS_HPP_
#define XTRAS_ENUMS_HPP_

#include <type_traits>
#include <functional>

namespace xtras {
namespace enums {

    template<typename T>
    struct enable_for;

    namespace detail {

        template<typename T>
        using enabler = std::enable_if_t<std::is_enum_v<T> && std::is_default_constructible_v<enable_for<T>>>*;

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
} // namespace xtras

template<typename EnumT, xtras::enums::detail::enabler<EnumT> = nullptr>
constexpr auto operator |(EnumT v1, EnumT v2) noexcept -> EnumT
{
    return xtras::enums::detail::op<std::bit_or<>>(v1, v2);
}

template<typename EnumT, xtras::enums::detail::enabler<EnumT> = nullptr>
constexpr auto operator &(EnumT v1, EnumT v2) noexcept -> EnumT
{
    return xtras::enums::detail::op<std::bit_and<>>(v1, v2);
}

template<typename EnumT, xtras::enums::detail::enabler<EnumT> = nullptr>
constexpr auto operator ^(EnumT v1, EnumT v2) noexcept -> EnumT
{
    return xtras::enums::detail::op<std::bit_xor<>>(v1, v2);
}

template<typename EnumT, xtras::enums::detail::enabler<EnumT> = nullptr>
constexpr auto operator ~(EnumT v1) noexcept -> EnumT
{
    return xtras::enums::detail::op<std::bit_not<>>(v1);
}

#endif // XTRAS_ENUMS_HPP_
