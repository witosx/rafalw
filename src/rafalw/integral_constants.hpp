#ifndef RAFALW_INTEGRAL_CONSTANTS_HPP_
#define RAFALW_INTEGRAL_CONSTANTS_HPP_

#include <type_traits>

inline namespace rafalw {
namespace integral_constants {

namespace detail {

    template<template<typename T1, typename T2> class Op, typename... T>
    struct ApplyT;

    template<template<typename T1, typename T2> class Op, typename T>
    struct ApplyT<Op, T>
    {
        using Result = T;
    };

    template<template<typename T1, typename T2> class Op, typename T1, typename T2, typename... Rest>
    struct ApplyT<Op, T1, T2, Rest...>
    {
        using Result = typename ApplyT<Op, Op<T1, T2>, Rest...>::Result;
    };

    template<template<typename T1, typename T2> class Op, typename... T>
    using Apply = typename ApplyT<Op, T...>::Result;

}

template<typename T1, typename T2>
using sum = std::integral_constant<decltype(T1() + T2()), T1() + T2()>;

template<typename T1, typename T2>
using diff = std::integral_constant<decltype(T1() - T2()), T1() - T2()>;

template<typename T1, typename T2>
using multiply = std::integral_constant<decltype(T1() * T2()), T1() * T2()>;

template<typename T1, typename T2>
using divide = std::integral_constant<decltype(T1() / T2()), T1() / T2()>;

template<typename T1, typename T2>
using logical_or = std::integral_constant<decltype(T1() || T2()), T1() || T2()>;

template<typename T1, typename T2>
using logical_and = std::integral_constant<decltype(T1() && T2()), T1() && T2()>;


template<typename... Ints>
using multi_sum = detail::Apply<sum, Ints...>;

template<typename... Ints>
using multi_diff = detail::Apply<diff, Ints...>;

template<typename... Ints>
using multi_logical_or = detail::Apply<logical_or, Ints...>;

template<typename... Ints>
using multi_logical_and = detail::Apply<logical_and, Ints...>;


template<typename IC, typename Int, Int V>
using add = sum<IC, std::integral_constant<Int, V>>;

template<typename IC, typename Int, Int V>
using sub = diff<IC, std::integral_constant<Int, V>>;

template<typename IC, typename Int, Int V>
using mul = multiply<IC, std::integral_constant<Int, V>>;

template<typename IC, typename Int, Int V>
using div = divide<IC, std::integral_constant<Int, V>>;

template<typename IC>
using inc = add<IC, typename IC::value_type, 1>;

template<typename IC>
using dec = sub<IC, typename IC::value_type, 1>;

} // namespace integral_constants
} // namespace rafalw

#endif // RAFALW_INTEGRAL_CONSTANTS_HPP_
