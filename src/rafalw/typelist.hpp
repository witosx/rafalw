#ifndef RAFALW_TYPELIST_HPP_
#define RAFALW_TYPELIST_HPP_

#include <rafalw/templates/compose.hpp>
#include <rafalw/utils/Type.hpp>
#include <type_traits>
#include <typeindex>
#include <typeinfo>

inline namespace rafalw {
namespace typelist {

namespace detail {

    template<bool... Args>
    struct AnyOp;

    template<>
    struct AnyOp<> : public std::integral_constant<bool, false> {};

    template<bool Arg, bool... Args>
    struct AnyOp<Arg, Args...> : public std::integral_constant<bool, Arg || AnyOp<Args...>::value> {};

    template<bool... Args>
    constexpr auto any = AnyOp<Args...>::value;

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


    template<typename Op>
    using result = typename Op::Result;

    template<typename T>
    struct IsInstance;

    template<typename L>
    struct Length;

    template<typename L>
    struct Tail;

    template<typename L>
    struct Head;

    template<typename L, typename Element>
    struct Append;

    template<typename L, typename Element>
    struct Prepend;

    template<typename L, template<typename> class... F>
    struct Map;

    template<typename L, template<typename, typename> class F>
    struct Reduce;

    template<typename L1, typename L2>
    struct Concat2;

    template<typename L>
    struct Reverse;

    template<typename L, typename Element, std::size_t I>
    struct Find;

    template<typename L, template<typename...> class F>
    struct Filter;

    template<typename L>
    struct Unique;

    template<typename L, std::size_t N>
    struct Drop;

    template<bool empty, typename... Ls>
    struct Zip;

    template<typename L, typename F>
    auto do_apply(F&& f, std::true_type) -> void;

    template<typename L, typename F>
    auto do_apply(F&& f, std::false_type) -> void;

    template<typename L1, typename L2>
    using concat2 = result<Concat2<L1, L2>>;

} // namespace detail

template<typename... Types>
struct List {};

struct NotFound {};

template<std::size_t V>
struct Index : public std::integral_constant<std::size_t, V> {};

// typelist functions

template<typename T>
constexpr auto isinstance = detail::IsInstance<T>::value;

template<typename List>
constexpr auto length = detail::Length<List>::value;

template<typename L>
constexpr auto isempty = length<L> == 0;

using empty = List<>;

template<typename L>
using tail = detail::result<detail::Tail<L>>;

template<typename L>
using head = detail::result<detail::Head<L>>;

template<typename L, typename E>
using append = detail::result<detail::Append<L, E>>;

template<typename L, typename E>
using prepend = detail::result<detail::Prepend<L, E>>;

template<typename L, template<typename> class... F>
using map = detail::result<detail::Map<L, F...>>;

template<typename L, template<typename, typename> class F>
using reduce = detail::result<detail::Reduce<L, F>>;

template<typename... Ls>
using concat = reduce<List<empty, Ls...>, detail::concat2>;

template<typename L>
using reverse = detail::result<detail::Reverse<L>>;

template<typename L, typename E, std::size_t I = 0>
using find = detail::result<detail::Find<L, E, I>>;

template<typename L, template<typename...> class F>
using filter = detail::result<detail::Filter<L, F>>;

template<typename L, typename E>
using remove = filter<L, detail::Bind<detail::Not<std::is_same>::template Result, E>::template Result>;

template<typename L>
using unique = detail::result<detail::Unique<L>>;

template<typename L, std::size_t N>
using drop = detail::result<detail::Drop<L, N>>;

template<typename L, std::size_t N>
using take = reverse<drop<reverse<L>, length<L> - N>>;

template<typename L, std::size_t I>
using get = head<drop<L, I>>;

template<typename L, std::size_t I>
using erase = concat<take<L, I>, drop<L, I + 1>>;

template<typename... Ls>
using zip = detail::result<detail::Zip<detail::any<isempty<Ls>...>, Ls...>>;

template<typename L, typename E>
constexpr auto contains = !std::is_same<find<L, E>, NotFound>::value;

template<typename L, typename E>
constexpr auto count = length<filter<L, detail::Bind<std::is_same, E>::template Result>>;

template<typename L, typename F>
auto apply(F&& f) -> void
{
    detail::do_apply<L>(std::forward<F>(f), std::integral_constant<bool, !isempty<L>>{});
}

namespace detail {

    template<typename T>
    struct IsInstance : public std::false_type {};

    template<typename... E>
    struct IsInstance<List<E...>> : public std::true_type {};

    template<typename... Elements>
    struct Length<List<Elements...>> : public std::integral_constant<std::size_t, sizeof...(Elements)> {};

    template<typename Element0, typename... Elements>
    struct Tail<List<Element0, Elements...>>
    {
        using Result = List<Elements...>;
    };

    template<typename Element0, typename... Elements>
    struct Head<List<Element0, Elements...>>
    {
        using Result = Element0;
    };

    template<typename... Elements, typename Element>
    struct Append<List<Elements...>, Element>
    {
        using Result = List<Elements..., Element>;
    };

    template<typename... Elements, typename Element>
    struct Prepend<List<Elements...>, Element>
    {
        using Result = List<Element, Elements...>;
    };

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

    template<typename... Elements1, typename... Elements2>
    struct Concat2<List<Elements1...>, List<Elements2...>>
    {
        using Result = List<Elements1..., Elements2...>;
    };

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

    template<typename L, typename F>
    auto do_apply(F&& f, std::true_type) -> void
    {
        f(utils::type<head<L>>);
        apply<tail<L>>(std::forward<F>(f));
    }

    template<typename L, typename F>
    auto do_apply(F&&, std::false_type) -> void {}

} // namespace detail

} // namespace typelist
} // namespace rafalw

#endif // RAFALW_TYPELIST_HPP_
