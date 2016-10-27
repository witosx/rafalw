#ifndef RAFALW_TEMPLATES_COMPOSE_HPP_
#define RAFALW_TEMPLATES_COMPOSE_HPP_

inline namespace rafalw {
namespace templates {

namespace detail {

    template<template<typename> class... Args>
    struct Compose;

    template<template<typename> class Arg, template<typename> class... Args>
    struct Compose<Arg, Args...>
    {
        template<typename T>
        using Result = typename Compose<Args...>::template Result<Arg<T>>;
    };

    template<>
    struct Compose<>
    {
        template<typename T>
        using Result = T;
    };

} // namespace detail

template<typename T, template<typename> class... Args>
using compose = typename detail::Compose<Args...>::template Result<T>;

} // namespace templates
} // namespace rafalw

#endif // SRC_RAFALW_TEMPLATES_COMPOSE_HPP_
