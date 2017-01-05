#ifndef RAFALW_TYPEMAP_HPP_
#define RAFALW_TYPEMAP_HPP_

#include <rafalw/typelist.hpp>

inline namespace rafalw {
namespace typemap {

namespace detail {

    struct NoDefault {};

    template<typename T>
    using result = typename T::Result;

    template<typename I>
    struct ItemKey;

    template<typename I>
    struct ItemValue;

    template<typename L>
    struct FromList;

    template<typename M>
    struct Items;

    template<typename M, typename K, typename D>
    struct Get;

    template<typename M, typename K, typename V>
    struct Set;
}

struct NotFound {};


template<typename K, typename V>
struct Item {};

template<typename I>
using item_key = detail::result<detail::ItemKey<I>>;

template<typename I>
using item_value = detail::result<detail::ItemValue<I>>;


template<typename... Items>
struct Map;

template<typename... Keys, typename... Values>
struct Map<Item<Keys, Values>...> {};


using empty = Map<>;

template<typename L>
using from_list = detail::result<detail::FromList<L>>;

template<typename M>
using items = detail::result<detail::Items<M>>;

template<typename M>
using keys = typelist::map<items<M>, item_key>;

template<typename M>
using values = typelist::map<items<M>, item_value>;

template<typename M, typename K, typename D = detail::NoDefault>
using get = detail::result<detail::Get<M, K, D>>;

template<typename M, typename K, typename V>
using set = detail::result<detail::Set<M, K, V>>;


template<typename M>
constexpr auto size = typelist::length<items<M>>;

template<typename M>
constexpr auto isempty = typelist::isempty<items<M>>;

template<typename M, typename K>
constexpr auto contains = typelist::contains<keys<M>, K>;





namespace detail {

    template<typename K, typename V>
    struct ItemKey<Item<K, V>>
    {
        using Result = K;
    };

    template<typename K, typename V>
    struct ItemValue<Item<K, V>>
    {
        using Result = V;
    };

    template<typename... Items>
    struct FromList<typelist::List<Items...>>
    {
        using Result = Map<Items...>;
    };

    template<typename... ItemsT>
    struct Items<Map<ItemsT...>>
    {
        using Result = typelist::List<ItemsT...>;
    };


    template<typename M, typename Idx, typename Def>
    struct GetIndex;

    template<typename M, typename Def>
    struct GetIndex<M, typelist::NotFound, Def>
    {
        using Result = Def;
    };

    template<typename M, typename Def, std::size_t I>
    struct GetIndex<M, typelist::Index<I>, Def>
    {
        using Result = item_value<typelist::get<items<M>, I>>;
    };

    template<typename M, typename K, typename Def>
    struct GetDefault
    {
        using Idx = typelist::find<keys<M>, K>;
        using Result = result<GetIndex<M, Idx, Def>>;
    };

    template<typename M, typename K, typename D>
    struct Get
    {
        using Result = result<GetDefault<M, K, D>>;
    };

    template<typename M, typename K>
    struct Get<M, K, NoDefault>
    {
        using Result = result<GetDefault<M, K, NotFound>>;
        static_assert(!std::is_same<Result, NotFound>::value, "key not found");
    };



    template<typename M, typename Idx, typename K, typename V>
    struct SetIndex;

    template<typename M, typename K, typename V>
    struct SetIndex<M, typelist::NotFound, K, V>
    {
        using Result = from_list<typelist::append<items<M>, Item<K, V>>>;
    };

    template<typename M, typename K, typename V, std::size_t I>
    struct SetIndex<M, typelist::Index<I>, K, V>
    {
        using L = items<M>;
        using L1 = typelist::take<L, I>;
        using L2 = typelist::drop<L, I + 1>;
        using L3 = typelist::concat<typelist::append<L1, Item<K, V>>, L2>;
        using Result = from_list<L3>;
    };

    template<typename M, typename K, typename V>
    struct Set
    {
        using Idx = typelist::find<keys<M>, K>;
        using Result = result<SetIndex<M, Idx, K, V>>;
    };
}



} // namespace typemap
} // namespace rafalw

#endif // RAFALW_TYPEMAP_HPP_
