#ifndef RAFALW_TEMPLATES_CHAIN_HPP_
#define RAFALW_TEMPLATES_CHAIN_HPP_

inline namespace rafalw {
namespace templates {

namespace detail {

    template<template<typename...> class Element0, template<typename...> class... Elements>
    struct Chain
    {
        template<typename T>
        struct base
        {
            template<typename... Args>
            using args = Element0<Args..., typename Chain<Elements...>::template base<T>::template args<Args...>>;
        };

        template<typename... Args>
        using args = Element0<Args..., typename Chain<Elements...>::template args<Args...>>;
    };

    template<template<typename...> class Element0>
    struct Chain<Element0>
    {
        template<typename T>
        struct base
        {
            template<typename... Args>
            using args = Element0<Args..., T>;
        };

        template<typename... Args>
        using args = Element0<Args...>;
    };

} // namespace detail

template<template<typename...> class... Elements>
using chain = detail::Chain<Elements...>;

template<typename T, template<typename> class... Args>
using compose = typename detail::Compose<Args...>::Result<T>;

} // namespace templates
} // namespace rafalw

#endif // RAFALW_TEMPLATES_CHAIN_HPP_
