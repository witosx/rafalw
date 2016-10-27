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

    template<typename T>
    using ResultT = typename T::Result;

    template<typename T>
    constexpr auto ResultV = T::Result;

    // length

    template<typename L>
    struct LengthV;

    template<typename... Elements>
    struct LengthV<List<Elements...>>
    {
        static constexpr auto Result = sizeof...(Elements);
    };

    template<typename List>
    static constexpr auto Length = ResultV<LengthV<List>>;


    // tail

    template<typename L>
    struct TailT;

    template<typename Element0, typename... Elements>
    struct TailT<List<Element0, Elements...>>
    {
        using Result = List<Elements...>;
    };

    template<typename List>
    using Tail = ResultT<TailT<List>>;


    // head

    template<typename L>
    struct HeadT;

    template<typename Element0, typename... Elements>
    struct HeadT<List<Element0, Elements...>>
    {
        using Result = Element0;
    };

    template<typename List>
    using Head = ResultT<HeadT<List>>;


    // append

    template<typename L, typename Element>
    struct AppendT;

    template<typename... Elements, typename Element>
    struct AppendT<List<Elements...>, Element>
    {
        using Result = List<Elements..., Element>;
    };

    template<typename List, typename Element>
    using Append = ResultT<AppendT<List, Element>>;


    // prepend

    template<typename L, typename Element>
    struct PrependT;

    template<typename... Elements, typename Element>
    struct PrependT<List<Elements...>, Element>
    {
        using Result = List<Element, Elements...>;
    };

    template<typename List, typename Element>
    using Prepend = ResultT<PrependT<List, Element>>;


    // concat

    template<typename L1, typename L2>
    struct ConcatT;

    template<typename... Elements1, typename... Elements2>
    struct ConcatT<List<Elements1...>, List<Elements2...>>
    {
        using Result = List<Elements1..., Elements2...>;
    };

    template<typename L1, typename L2>
    using Concat = ResultT<ConcatT<L1, L2>>;


    // reverse

    template<typename L>
    struct ReverseT
    {
        using Result = Append<ResultT<ReverseT<Tail<L>>>, Head<L>>;
    };

    template<>
    struct ReverseT<empty>
    {
        using Result = empty;
    };

    template<typename List>
    using Reverse = ResultT<ReverseT<List>>;


    // count

    template<typename L, typename Element>
    struct CountT
    {
        static constexpr auto Result = std::is_same<Element, Head<L>>::value + CountT<Tail<L>, Element>::Result;
    };

    template<typename E>
    struct CountT<empty, E>
    {
        static constexpr auto Result = std::size_t{ 0 };
    };

    template<typename L, typename E>
    static constexpr auto Count = ResultV<CountT<L, E>>;


    // find

    template<typename L, typename Element, std::size_t I = 0>
    struct FindT
    {
        using Result = ResultT<FindT<Tail<L>, Element, I + 1>>;
    };

    template<typename... Elements, typename Element, std::size_t I>
    struct FindT<List<Element, Elements...>, Element, I>
    {
        using Result = Index<I>;
    };

    template<typename Element, std::size_t I>
    struct FindT<empty, Element, I>
    {
        using Result = NotFound;
    };

    template<typename List, typename Element>
    using Find = typename FindT<List, Element>::Result;


    // contains

    template<typename List, typename Element>
    constexpr auto Contains = !std::is_same<Find<List, Element>, NotFound>::value;


    // drop

    template<typename L, std::size_t N>
    struct DropT
    {
        using Result = ResultT<DropT<Tail<L>, N - 1>>;
    };

    template<typename L>
    struct DropT<L, 0>
    {
        using Result = L;
    };

    template<typename List, std::size_t COUNT>
    using Drop = ResultT<DropT<List, COUNT>>;


    // take

    template<typename L, std::size_t COUNT>
    using Take = Reverse<Drop<Reverse<L>, Length<L> - COUNT>>;


    // element

    template<typename L, std::size_t N>
    using Element = Head<Drop<L, N>>;


    // erase

    template<typename L, std::size_t INDEX>
    using Erase = Concat<Take<L, INDEX>, Drop<L, INDEX + 1>>;


    // unique

    template<typename L, typename Res = empty>
    struct UniqueT
    {
        using Result = ResultT<UniqueT<Tail<L>, std::conditional_t<Contains<Res, Head<L>>, Res, Append<Res, Head<L>>>>>;
    };

    template<typename Res>
    struct UniqueT<empty, Res>
    {
        using Result = Res;
    };

    template<typename List>
    using Unique = ResultT<UniqueT<List>>;


    // map

    template<typename L, template<typename> class... F>
    struct MapT
    {
        using Result = Prepend<ResultT<MapT<Tail<L>, F...>>, templates::compose<Head<L>, F...>>;
    };

    template<template<typename> class... F>
    struct MapT<empty, F...>
    {
        using Result = empty;
    };

    template<typename L, template<typename> class... F>
    using Map = ResultT<MapT<L, F...>>;

    template<typename L, template<typename, typename> class F>
    struct AccumulateT
    {
        using Result = F<Head<L>, ResultT<AccumulateT<Tail<L>, F>>>;
    };

    template<typename E, template<typename, typename> class F>
    struct AccumulateT<List<E>, F>
    {
        using Result = E;
    };

    template<template<typename, typename> class F>
    struct AccumulateT<empty, F>
    {};

    template<typename L, template<typename, typename> class F>
    using Accumulate = ResultT<AccumulateT<L, F>>;

} // namespace detail

template<typename List>
static constexpr auto length = detail::Length<List>;

template<typename List>
static constexpr auto isempty = length<List> == 0;

template<typename List, typename Element>
static constexpr auto contains = detail::Contains<List, Element>;

template<typename List, typename Element>
static constexpr auto count = detail::Count<List, Element>;

template<typename List, std::size_t INDEX>
using element = detail::Element<List, INDEX>;

template<typename List>
using head = detail::Head<List>;

template<typename List>
using tail = detail::Tail<List>;

template<typename List, typename Element>
using append = detail::Append<List, Element>;

template<typename List, typename Element>
using prepend = detail::Prepend<List, Element>;

template<typename List>
using reverse = detail::Reverse<List>;

template<typename List1, typename List2>
using concat = detail::Concat<List1, List2>;

template<typename List, typename Element>
using find = detail::Find<List, Element>;

template<typename List>
using unique = detail::Unique<List>;

template<typename List, std::size_t COUNT>
using drop = detail::Drop<List, COUNT>;

template<typename List, std::size_t COUNT>
using take = detail::Take<List, COUNT>;

template<typename L, std::size_t INDEX>
using erase = detail::Erase<L, INDEX>;

template<typename L, template<typename> class... F>
using map = detail::Map<L, F...>;

template<typename L, template<typename, typename> class F>
using accumulate = detail::Accumulate<L, F>;

} // namespace typelist
} // namespace rafalw

#endif // RAFALW_TYPELIST_HPP_
