#ifndef RAFALW_TYPEMAP_HPP_
#define RAFALW_TYPEMAP_HPP_

#include <rafalw/typelist.hpp>

inline namespace rafalw {
namespace typemap {

namespace detail {

    struct NoDefault {};

    template<typename Op>
    using result = typename Op::Result;

    template<typename I>
    struct ItemKeyOp;

    template<typename I>
    struct ItemValueOp;

    template<typename L>
    struct FromListOp;

    template<typename M>
    struct ItemsOp;

    template<typename M, typename K, typename D>
    struct GetOp;

    template<typename M, typename K, typename V>
    struct SetOp;
}

struct NotFound {};


template<typename K, typename V>
struct Item {};

template<typename I>
using item_key = detail::result<detail::ItemKeyOp<I>>;

template<typename I>
using item_value = detail::result<detail::ItemValueOp<I>>;


template<typename... Items>
struct Map;

using empty = Map<>;

template<typename L>
using from_list = detail::result<detail::FromListOp<L>>;

template<typename M>
using items = detail::result<detail::ItemsOp<M>>;

template<typename M>
using keys = typelist::map<items<M>, item_key>;

template<typename M>
using values = typelist::map<items<M>, item_value>;

template<typename M, typename K, typename D = detail::NoDefault>
using get = detail::result<detail::GetOp<M, K, D>>;

template<typename M, typename K, typename V>
using set = detail::result<detail::SetOp<M, K, V>>;


template<typename M>
constexpr auto size = typelist::length<items<M>>;

template<typename M>
constexpr auto isempty = typelist::isempty<items<M>>;

template<typename M, typename K>
constexpr auto contains = typelist::contains<keys<M>, K>;


template<typename... Keys, typename... Values>
struct Map<Item<Keys, Values>...> {};


namespace detail {

    template<typename K, typename V>
    struct ItemKeyOp<Item<K, V>>
    {
        using Result = K;
    };

    template<typename K, typename V>
    struct ItemValueOp<Item<K, V>>
    {
        using Result = V;
    };

    template<typename... Items>
    struct FromListOp<typelist::List<Items...>>
    {
        using Result = Map<Items...>;
    };

    template<typename... Items>
    struct ItemsOp<Map<Items...>>
    {
        using Result = typelist::List<Items...>;
    };


    template<typename M, typename Idx, typename Def>
    struct GetIndexOp;

    template<typename M, typename Def>
    struct GetIndexOp<M, typelist::NotFound, Def>
    {
        using Result = Def;
    };

    template<typename M, typename Def, std::size_t I>
    struct GetIndexOp<M, typelist::Index<I>, Def>
    {
        using Result = item_value<typelist::get<items<M>, I>>;
    };

    template<typename M, typename K, typename Def>
    struct GetDefaultOp
    {
        using Idx = typelist::find<keys<M>, K>;
        using Result = result<GetIndexOp<M, Idx, Def>>;
    };

    template<typename M, typename K, typename D>
    struct GetOp
    {
        using Result = result<GetDefaultOp<M, K, D>>;
    };

    template<typename M, typename K>
    struct GetOp<M, K, NoDefault>
    {
        using Result = result<GetDefaultOp<M, K, NotFound>>;
        static_assert(!std::is_same<Result, NotFound>::value, "key not found");
    };



    template<typename M, typename Idx, typename K, typename V>
    struct SetIndexOp;

    template<typename M, typename K, typename V>
    struct SetIndexOp<M, typelist::NotFound, K, V>
    {
        using Result = from_list<typelist::append<items<M>, Item<K, V>>>;
    };

    template<typename M, typename K, typename V, std::size_t I>
    struct SetIndexOp<M, typelist::Index<I>, K, V>
    {
        using L = items<M>;
        using L1 = typelist::take<L, I>;
        using L2 = typelist::drop<L, I + 1>;
        using L3 = typelist::concat<typelist::append<L1, Item<K, V>>, L2>;
        using Result = from_list<L3>;
    };

    template<typename M, typename K, typename V>
    struct SetOp
    {
        using Idx = typelist::find<keys<M>, K>;
        using Result = result<SetIndexOp<M, Idx, K, V>>;
    };
}



} // namespace typemap
} // namespace rafalw

#endif // RAFALW_TYPEMAP_HPP_
