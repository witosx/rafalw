#ifndef RAFALW_TYPEMAP_HPP_
#define RAFALW_TYPEMAP_HPP_

#include <rafalw/typelist.hpp>

inline namespace rafalw {
namespace typemap {

struct NotFound {};

template<typename K, typename V>
struct Item
{
    using Key = K;
    using Value = V;
};

template<typename _Items>
struct Map
{
    using Items = _Items;
};

template<typename... Items>
using create = Map<typelist::List<Items...>>;

using empty = create<>;

namespace detail {

    struct NoDefault {};

    template<typename I>
    struct ItemKeyT;

    template<typename K, typename V>
    struct ItemKeyT<Item<K, V>>
    {
        using Result = K;
    };

    template<typename I>
    using ItemKey = typename ItemKeyT<I>::Result;



    template<typename I>
    struct ItemValueT;

    template<typename K, typename V>
    struct ItemValueT<Item<K, V>>
    {
        using Result = V;
    };

    template<typename I>
    using ItemValue = typename ItemValueT<I>::Result;




    template<typename M>
    using Items = typename M::Items;

    template<typename M>
    using Keys = typelist::map<Items<M>, ItemKey>;

    template<typename M>
    using Values = typelist::map<Items<M>, ItemValue>;

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
        using Result = ItemValue<typelist::element<Items<M>, I>>;
    };

    template<typename M, typename K, typename Def>
    struct GetDefaultT
    {
        using Idx = typelist::find<Keys<M>, K>;
        using Result = typename GetIndex<M, Idx, Def>::Result;
    };

    template<typename M, typename K, typename Def>
    using GetDefault = typename GetDefaultT<M, K, Def>::Result;

    template<typename M, typename K, typename D>
    struct GetT
    {
        using Result = GetDefault<M, K, D>;
    };

    template<typename M, typename K>
    struct GetT<M, K, NoDefault>
    {
        using Result = GetDefault<M, K, NotFound>;
        static_assert(!std::is_same<Result, NotFound>::value, "key not found");
    };

    template<typename M, typename K, typename D>
    using Get = typename GetT<M, K, D>::Result;


    template<typename M, typename Idx, typename K, typename V>
    struct SetIndex;

    template<typename M, typename K, typename V>
    struct SetIndex<M, typelist::NotFound, K, V>
    {
        using Result = Map<typelist::append<Items<M>, Item<K, V>>>;
    };

    template<typename M, typename K, typename V, std::size_t I>
    struct SetIndex<M, typelist::Index<I>, K, V>
    {
        using L = Items<M>;
        using L1 = typelist::take<L, I>;
        using L2 = typelist::drop<L, I + 1>;
        using L3 = typelist::concat<typelist::append<L1, Item<K, V>>, L2>;
        using Result = Map<L3>;
    };



    template<typename M, typename K, typename V>
    struct SetT
    {
        using Idx = typelist::find<Keys<M>, K>;
        using Result = typename SetIndex<M, Idx, K, V>::Result;
    };

    template<typename M, typename K, typename V>
    using Set = typename SetT<M, K, V>::Result;

    template<typename M, typename K>
    constexpr auto Contains = typelist::contains<Keys<M>, K>;
}

template<typename M>
using items = detail::Items<M>;

template<typename M>
using keys = detail::Keys<M>;

template<typename M>
using values = detail::Values<M>;

template<typename M, typename K, typename V>
using set = detail::Set<M, K, V>;

template<typename M, typename K, typename D = detail::NoDefault>
using get = detail::Get<M, K, D>;

template<typename M>
constexpr auto size = typelist::length<items<M>>;

template<typename M>
constexpr auto isempty = typelist::isempty<items<M>>;

template<typename M, typename K>
constexpr auto contains = detail::Contains<M, K>;

} // namespace typemap
} // namespace rafalw

#endif // RAFALW_TYPEMAP_HPP_
