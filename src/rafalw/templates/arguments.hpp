#ifndef RAFALW_TEMPLATES_ARGUMENTS_HPP_
#define RAFALW_TEMPLATES_ARGUMENTS_HPP_

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

} // namespace detail

template<typename T>
using arguments = typename detail::Arguments<T>::All;

template<typename T, std::size_t IDX>
using argument = typelist::element<arguments<T>, IDX>;

template<typename T>
static constexpr auto arguments_count = typelist::length<arguments<T>>;

} // namespace templates
} // namespace rafalw

#endif // SRC_RAFALW_TEMPLATES_ARGUMENTS_HPP_
