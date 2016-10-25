#ifndef RAFALW_TEMPLATES_HPP_
#define RAFALW_TEMPLATES_HPP_

#include <rafalw/typelist.hpp>

inline namespace rafalw {
namespace templates {

namespace detail {

    template<typename... Args>
    struct ArgumentsBase
    {
        using All = typelist::List<Args...>;
    };

    template<typename T>
    struct Arguments : public ArgumentsBase<> {};

    template<template<typename...> class Tmpl, typename... Args>
    struct Arguments<Tmpl<Args...>> : public ArgumentsBase<Args...> {};

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

template<typename T>
using arguments = typename detail::Arguments<T>::All;

template<typename T, std::size_t IDX>
using argument = typelist::element<arguments<T>, IDX>;

template<typename T>
static constexpr auto arguments_count = typelist::length<arguments<T>>;

template<template<typename...> class... Elements>
using chain = detail::Chain<Elements...>;

} // namespace templates
} // namespace rafalw

#endif // RAFALW_TEMPLATES_HPP_
