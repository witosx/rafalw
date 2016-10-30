#ifndef RAFALW_TYPELIST_HPP_
#define RAFALW_TYPELIST_HPP_

#include <rafalw/templates/compose.hpp>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

inline namespace rafalw {
namespace typelist {

template<typename... Types>
struct List {};

using empty = List<>;

struct NotFound {};

template<std::size_t V>
struct Index : public std::integral_constant<std::size_t, V> {};

namespace detail {

    template<bool... Args>
    struct Any;

    template<>
    struct Any<> : public std::integral_constant<bool, false> {};

    template<bool Arg, bool... Args>
    struct Any<Arg, Args...> : public std::integral_constant<bool, Arg || Any<Args...>::value> {};

    template<bool... Args>
    constexpr auto any = Any<Args...>::value;

    template<typename B>
    struct Negate : public std::integral_constant<bool, !B::value> {};

    template<template<typename...> class F>
    struct Not
    {
        template<typename... T>
        using Result = Negate<F<T...>>;
    };

    template<template<typename...> class T, typename Arg>
    struct Bind
    {
        template<typename... Args>
        using Result = T<Arg, Args...>;
    };

    template<typename T>
    using result = typename T::Result;

    // isinstance

    template<typename T>
    struct IsInstance : public std::false_type {};

    template<typename... E>
    struct IsInstance<List<E...>> : public std::true_type {};

    template<typename T>
    constexpr auto isinstance = IsInstance<T>::value;

    // length

    template<typename L>
    struct Length;

    template<typename... Elements>
    struct Length<List<Elements...>> : public std::integral_constant<std::size_t, sizeof...(Elements)> {};

    template<typename List>
    constexpr auto length = Length<List>::value;

    // isempty

    template<typename L>
    constexpr auto isempty = length<L> == 0;

    // tail

    template<typename L>
    struct Tail;

    template<typename Element0, typename... Elements>
    struct Tail<List<Element0, Elements...>>
    {
        using Result = List<Elements...>;
    };

    template<typename List>
    using tail = result<Tail<List>>;


    // head

    template<typename L>
    struct Head;

    template<typename Element0, typename... Elements>
    struct Head<List<Element0, Elements...>>
    {
        using Result = Element0;
    };

    template<typename List>
    using head = result<Head<List>>;


    // append

    template<typename L, typename Element>
    struct Append;

    template<typename... Elements, typename Element>
    struct Append<List<Elements...>, Element>
    {
        using Result = List<Elements..., Element>;
    };

    template<typename List, typename Element>
    using append = result<Append<List, Element>>;


    // prepend

    template<typename L, typename Element>
    struct Prepend;

    template<typename... Elements, typename Element>
    struct Prepend<List<Elements...>, Element>
    {
        using Result = List<Element, Elements...>;
    };

    template<typename List, typename Element>
    using prepend = result<Prepend<List, Element>>;

    // map

    template<typename L, template<typename> class... F>
    struct Map;

    template<typename L, template<typename> class... F>
    using map = result<Map<L, F...>>;

    template<typename L, template<typename> class... F>
    struct Map
    {
        using Result = prepend<map<tail<L>, F...>, templates::compose<head<L>, F...>>;
    };

    template<template<typename> class... F>
    struct Map<empty, F...>
    {
        using Result = empty;
    };

    // reduce

    template<typename L, template<typename, typename> class F>
    struct Reduce;

    template<typename L, template<typename, typename> class F>
    using reduce = result<Reduce<L, F>>;

    template<typename E1, typename E2, typename... E, template<typename, typename> class F>
    struct Reduce<List<E1, E2, E...>, F>
    {
        using Result = reduce<List<F<E1, E2>, E...>, F>;
    };

    template<typename E, template<typename, typename> class F>
    struct Reduce<List<E>, F>
    {
        using Result = E;
    };

    // concat

    template<typename L1, typename L2>
    struct Concat2;

    template<typename... Elements1, typename... Elements2>
    struct Concat2<List<Elements1...>, List<Elements2...>>
    {
        using Result = List<Elements1..., Elements2...>;
    };

    template<typename L1, typename L2>
    using concat2 = result<Concat2<L1, L2>>;

    template<typename... L>
    using concat = reduce<List<empty, L...>, concat2>;

    // reverse

    template<typename L>
    struct Reverse;

    template<typename List>
    using reverse = result<Reverse<List>>;

    template<typename L>
    struct Reverse
    {
        using Result = append<reverse<tail<L>>, head<L>>;
    };

    template<>
    struct Reverse<empty>
    {
        using Result = empty;
    };

    // find

    template<typename L, typename Element, std::size_t I>
    struct Find;

    template<typename List, typename Element, std::size_t I = 0>
    using find = result<Find<List, Element, I>>;

    template<typename L, typename Element, std::size_t I>
    struct Find
    {
        using Result = find<tail<L>, Element, I + 1>;
    };

    template<typename... Elements, typename Element, std::size_t I>
    struct Find<List<Element, Elements...>, Element, I>
    {
        using Result = Index<I>;
    };

    template<typename Element, std::size_t I>
    struct Find<empty, Element, I>
    {
        using Result = NotFound;
    };

    // contains

    template<typename List, typename Element>
    constexpr auto contains = !std::is_same<find<List, Element>, NotFound>::value;

    // filter

    template<typename L, template<typename...> class F>
    struct Filter;

    template<typename L, template<typename...> class F>
    using filter = result<Filter<L, F>>;

    template<typename L, template<typename...> class F>
    struct Filter
    {
        using H = head<L>;
        using Result = concat<std::conditional_t<F<H>::value, List<H>, empty>, filter<tail<L>, F>>;
    };

    template<template<typename...> class F>
    struct Filter<empty, F>
    {
        using Result = empty;
    };

    // remove

    template<typename L, typename E>
    using remove = filter<L, Bind<Not<std::is_same>::template Result, E>::template Result>;

    // count

    template<typename L, typename E>
    static constexpr auto count = length<filter<L, Bind<std::is_same, E>::template Result>>;

    // unique

    template<typename L>
    struct Unique;

    template<typename L>
    using unique = result<Unique<L>>;

    template<typename L>
    struct Unique
    {
        using Result = prepend<unique<remove<L, head<L>>>, head<L>>;
    };

    template<>
    struct Unique<empty>
    {
        using Result = empty;
    };

    // drop

    template<typename L, std::size_t N>
    struct Drop;

    template<typename List, std::size_t COUNT>
    using drop = result<Drop<List, COUNT>>;

    template<typename L, std::size_t N>
    struct Drop
    {
        using Result = drop<tail<L>, N - 1>;
    };

    template<typename L>
    struct Drop<L, 0>
    {
        using Result = L;
    };

    // take

    template<typename L, std::size_t COUNT>
    using take = reverse<drop<reverse<L>, length<L> - COUNT>>;

    // element

    template<typename L, std::size_t N>
    using get = head<drop<L, N>>;

    // erase

    template<typename L, std::size_t INDEX>
    using erase = concat<take<L, INDEX>, drop<L, INDEX + 1>>;

    // zip

    template<bool empty, typename... Ls>
    struct Zip;

    template<typename... Ls>
    using zip = result<Zip<any<isempty<Ls>...>, Ls...>>;

    template<typename... Ls>
    struct Zip<true, Ls...>
    {
        using Result = empty;
    };

    template<typename... Ls>
    struct Zip<false, Ls...>
    {
        using Result = concat<List<List<head<Ls>...>>, zip<tail<Ls>...>>;
    };

} // namespace detail

template<typename T>
static constexpr auto isinstance = detail::isinstance<T>;

template<typename L>
static constexpr auto length = detail::length<L>;

template<typename L>
static constexpr auto isempty = detail::isempty<L>;

template<typename L, typename E>
static constexpr auto contains = detail::contains<L, E>;

template<typename L, typename E>
static constexpr auto count = detail::count<L, E>;

template<typename L, std::size_t I>
using get = detail::get<L, I>;

template<typename L>
using head = detail::head<L>;

template<typename L>
using tail = detail::tail<L>;

template<typename L, typename E>
using append = detail::append<L, E>;

template<typename L, typename E>
using prepend = detail::prepend<L, E>;

template<typename L>
using reverse = detail::reverse<L>;

template<typename... L>
using concat = detail::concat<L...>;

template<typename L, typename E>
using find = detail::find<L, E>;

template<typename L>
using unique = detail::unique<L>;

template<typename L, std::size_t N>
using drop = detail::drop<L, N>;

template<typename L, std::size_t N>
using take = detail::take<L, N>;

template<typename L, std::size_t Idx>
using erase = detail::erase<L, Idx>;

template<typename L, typename E>
using remove = detail::remove<L, E>;

template<typename L, template<typename> class F>
using filter = detail::filter<L, F>;

template<typename L, template<typename> class... F>
using map = detail::map<L, F...>;

template<typename L, template<typename, typename> class F>
using reduce = detail::reduce<L, F>;

template<typename... Ls>
using zip = detail::zip<Ls...>;

} // namespace typelist
} // namespace rafalw

#endif // RAFALW_TYPELIST_HPP_
