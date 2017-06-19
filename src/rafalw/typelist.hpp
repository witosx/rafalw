#ifndef RAFALW_TYPELIST_HPP_
#define RAFALW_TYPELIST_HPP_

#include <type_traits>
#include <utility>

inline namespace rafalw {
namespace typelist {

namespace detail {

    template<template<typename> class Unary>
    struct Negate
    {
        template<typename Arg1>
        using function = typename std::negation<Unary<Arg1>>::type;
    };

    template<template<typename, typename> class Binary, typename Arg1>
    struct Bind
    {
        template<typename Arg2>
        using function = Binary<Arg1, Arg2>;
    };

    template<typename Op>
    using result = typename Op::Result;

    template<typename T>
    struct IsInstance;

    template<typename L>
    struct Length;

    template<typename L>
    struct Head;

    template<typename L>
    struct Tail;

    template<typename L, typename Element>
    struct Append;

    template<typename L, typename Element>
    struct Prepend;

    template<typename L, template<typename> class F>
    struct Map;

    template<typename L, template<typename, typename> class F>
    struct Reduce;

    template<typename L1, typename L2>
    struct Concat2;

    template<typename L>
    struct Reverse;

    template<typename L, typename Element, std::size_t I>
    struct Find;

    template<typename L, template<typename> class F>
    struct Filter;

    template<typename L>
    struct Unique;

    template<typename L, std::size_t N>
    struct Drop;

    template<bool empty, typename... Ls>
    struct Zip;

    template<typename L, typename E>
    struct Contains;

    template<typename L, typename E>
    struct Count;

    template<typename L1, typename L2>
    using concat2 = result<Concat2<L1, L2>>;

    template<typename L>
    struct Apply;



} // namespace detail

template<typename... Types>
struct List {};

struct NotFound {};

template<std::size_t V>
struct Index : public std::integral_constant<std::size_t, V> {};

template<typename E>
struct ElementWrapper
{
	using type = E;
};

// typelist functions

using empty = List<>;


template<typename T>
using is_instance = typename detail::IsInstance<T>::type;

template<typename T>
inline constexpr auto is_instance_v = is_instance<T>::value;


template<typename List>
using length = typename detail::Length<List>::type;

template<typename T>
inline constexpr auto length_v = length<T>::value;


template<typename L>
using is_empty = typename std::bool_constant<length<L>::value == 0>::type;

template<typename L>
inline constexpr auto is_empty_v = is_empty<L>::value;


template<typename L>
using head = detail::result<detail::Head<L>>;

template<typename L>
using tail = detail::result<detail::Tail<L>>;



template<typename L, typename E>
using append = detail::result<detail::Append<L, E>>;

template<typename L, typename E>
using prepend = detail::result<detail::Prepend<L, E>>;

template<typename L, template<typename> class F>
using map = detail::result<detail::Map<L, F>>;

template<typename L, template<typename, typename> class F>
using reduce = detail::result<detail::Reduce<L, F>>;

template<typename... Ls>
using concat = reduce<List<empty, Ls...>, detail::concat2>;

template<typename L>
using reverse = detail::result<detail::Reverse<L>>;

template<typename L, typename E, std::size_t I = 0>
using find = detail::result<detail::Find<L, E, I>>;

template<typename L, template<typename> class F>
using filter = detail::result<detail::Filter<L, F>>;

template<typename L, typename E>
using remove = filter<L, detail::Negate<detail::Bind<std::is_same, E>::template function>::template function>;

template<typename L>
using unique = detail::result<detail::Unique<L>>;

template<typename L, std::size_t N>
using drop = detail::result<detail::Drop<L, N>>;

template<typename L, std::size_t N>
using take = reverse<drop<reverse<L>, (length_v<L>) - N>>;

template<typename L, std::size_t I>
using get = head<drop<L, I>>;

template<typename L, std::size_t I>
using erase = concat<take<L, I>, drop<L, I + 1>>;

template<typename... Ls>
using zip = detail::result<detail::Zip<std::disjunction_v<is_empty<Ls>...>, Ls...>>;


template<typename L, typename E>
using count = detail::result<detail::Count<L, E>>;

template<typename L, typename E>
inline constexpr auto count_v = count<L, E>::value;


template<typename L, typename E>
using contains = detail::result<detail::Contains<L, E>>;

template<typename L, typename E>
inline constexpr auto contains_v = contains<L, E>::value;


template<typename L, typename F>
auto apply(F&& f) -> void
{
    detail::Apply<L>::apply(std::forward<F>(f));
}


namespace detail {

    template<typename T>
    struct IsInstance : public std::false_type {};

    template<typename... E>
    struct IsInstance<List<E...>> : public std::true_type {};

    template<typename... Elements>
    struct Length<List<Elements...>> : public std::integral_constant<std::size_t, sizeof...(Elements)> {};

    template<typename Element0, typename... Elements>
    struct Head<List<Element0, Elements...>>
    {
        using Result = Element0;
    };

    template<typename Element0, typename... Elements>
    struct Tail<List<Element0, Elements...>>
    {
        using Result = List<Elements...>;
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

    template<typename... Elements, template<typename> class F>
    struct Map<List<Elements...>, F>
    {
        using Result = List<F<Elements>...>;
    };

    template<typename... Elements1, typename... Elements2>
    struct Concat2<List<Elements1...>, List<Elements2...>>
    {
        using Result = List<Elements1..., Elements2...>;
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

    template<typename L, template<typename> class F>
    struct Filter
    {
        using H = head<L>;
        using Result = concat<std::conditional_t<F<H>::value, List<H>, empty>, filter<tail<L>, F>>;
    };

    template<template<typename> class F>
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

    template<typename... Elements, typename E>
    struct Count<List<Elements...>, E>
    {
    	using Result = std::integral_constant<std::size_t, (std::is_same_v<Elements, E> + ...)>;
    };

    template<typename... Elements, typename E>
    struct Contains<List<Elements...>, E>
    {
    	using Result = typename std::disjunction<std::is_same<Elements, E>...>::type;
    };

    template<typename... Elements>
    struct Apply<List<Elements...>>
    {
        template<typename F>
        static auto apply(F&& f) -> void
        {
            (f(ElementWrapper<Elements>{}), ...);
        }
    };

} // namespace detail

} // namespace typelist
} // namespace rafalw

#endif // RAFALW_TYPELIST_HPP_
