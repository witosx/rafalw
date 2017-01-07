#ifndef RAFALW_UNITS_HPP_
#define RAFALW_UNITS_HPP_

#include <rafalw/typelist.hpp>
#include <rafalw/typemap.hpp>
#include <ratio>

inline namespace rafalw {
namespace units {

namespace detail {

	template<typename T>
	using result = typename T::Result;

	template<typename T>
	struct IsUnit;

	template<typename UnitT>
	struct UnitTag;

	template<typename UnitT>
	struct UnitPow;

	template<typename T>
	struct IsUnitSet;

	template<typename UnitSetT>
	struct ToList;

	template<typename UnitSetT>
	struct ToMap;

	template<template<typename, typename> class Ratio, typename UnitSet1T, typename UnitSet2T>
	struct Binary;

	template<typename Us1, typename R>
	struct Pow;

} // namespace detail


// Tag

struct Tag {};

template<typename T>
using is_tag_t = typename std::is_base_of<Tag, T>::type;

template<typename T>
constexpr auto is_tag = is_tag_t<T>::value;

// Unit

template<typename Tag, typename Pow = std::ratio<1>>
struct Unit;

template<typename TagT, std::intmax_t PowN, std::intmax_t PowD>
struct Unit<TagT, std::ratio<PowN, PowD>>
{
	static_assert(is_tag<TagT>, "tag must be derived from UnitTag");
};

template<typename T>
using is_unit_t = typename detail::IsUnit<T>::type;

template<typename T>
constexpr auto is_unit = is_unit_t<T>::value;

template<typename U>
using unit_tag = detail::result<detail::UnitTag<U>>;

template<typename U>
using unit_pow = detail::result<detail::UnitPow<U>>;

// UnitSet

template<typename... U>
struct UnitSet;

template<typename... Tags, typename... Pows>
struct UnitSet<Unit<Tags, Pows>...> {};

using Null = UnitSet<>;

template<typename T>
using is_unitset_t = typename detail::IsUnitSet<T>::type;

template<typename T>
constexpr auto is_unitset = is_unitset_t<T>::value;

template<typename T>
using is_null_t = typename std::is_same<T, Null>::type;

template<typename T>
constexpr auto is_null = is_null_t<T>::value;

template<typename UnitSetT>
using to_list = detail::result<detail::ToList<UnitSetT>>;

template<typename UnitSetT>
using to_map = detail::result<detail::ToMap<UnitSetT>>;

template<typename Us1, typename Us2>
using mul = detail::result<detail::Binary<std::ratio_add, Us1, Us2>>;

template<typename Us1, typename Us2>
using div = detail::result<detail::Binary<std::ratio_subtract, Us1, Us2>>;

template<typename Us1, typename Pow>
using pow = detail::result<detail::Pow<Us1, Pow>>;

// other

template<typename T>
constexpr auto is_instance = is_tag<T> || is_unit<T> || is_unitset<T>;

template<typename T>
using is_instance_t = std::integral_constant<bool, is_instance<T>>;


namespace detail {

	template<typename TagT, typename PowT>
	struct UnitTag<Unit<TagT, PowT>>
	{
		using Result = TagT;
	};

	template<typename TagT, typename PowT>
	struct UnitPow<Unit<TagT, PowT>>
	{
		using Result = PowT;
	};

	template<typename... U>
	struct ToList<UnitSet<U...>>
	{
		using Result = typelist::List<U...>;
	};

	template<typename... U>
	struct ToMap<UnitSet<U...>>
	{
		using Result = typemap::Map<typemap::Item<unit_tag<U>, unit_pow<U>>...>;
	};

	template<typename L>
	struct FromList;

	template<typename... Es>
	struct FromList<typelist::List<Es...>>
	{
		using Result = UnitSet<Es...>;
	};

	template<typename L>
	using from_list = result<FromList<L>>;

	template<typename Us>
	struct Normalize
	{
		using Map = to_map<Us>;
		using Items = typemap::items<Map>;

		template<typename I>
		struct Pred : public std::ratio_not_equal<typemap::item_value<I>, std::ratio<0>> {};

		template<typename I>
		using Mod = Unit<typemap::item_key<I>, typemap::item_value<I>>;

		using ResultA = typelist::filter<Items, Pred>;
		using ResultB = typelist::map<ResultA, Mod>;

		using Result = from_list<ResultB>;
	};

	template<typename Us>
	using normalize = result<Normalize<Us>>;


	template<typename T, typename = std::enable_if_t<true>>
	struct ToUnitSet;

	template<typename T>
	using to_unit_set = result<ToUnitSet<T>>;


	template<typename... UnitsT>
	struct ToUnitSet<UnitSet<UnitsT...>>
	{
		using Result = UnitSet<UnitsT...>;
	};

	template<typename TagT, typename PowT>
	struct ToUnitSet<Unit<TagT, PowT>>
	{
		using Result = UnitSet<Unit<TagT, PowT>>;
	};

	template<typename T>
	struct ToUnitSet<T, std::enable_if_t<is_tag<T>>>
	{
		using Result = UnitSet<Unit<T>>;
	};



	template<typename T, typename = std::enable_if_t<true>>
	struct Simplify;

	template<typename T>
	using simplify = result<Simplify<T>>;


	template<typename T, typename>
	struct Simplify
	{
		using Result = T;
	};

	template<typename TagT>
	struct Simplify<Unit<TagT, std::ratio<1>>>
	{
		using Result = TagT;
	};

	template<typename U>
	struct Simplify<UnitSet<U>>
	{
		using Result = simplify<U>;
	};




	template<template<typename, typename> class RatioOp, typename LHS, typename RHS>
	struct Binary
	{
		using Us1 = to_unit_set<LHS>;
		using Us2 = to_unit_set<RHS>;

		using Map1 = to_map<Us1>;
		using Map2 = to_map<Us2>;

		using Tags1 = typemap::keys<Map1>;
		using Tags2 = typemap::keys<Map2>;

		using Tags = typelist::unique<typelist::concat<Tags1, Tags2>>;

		template<typename TagT>
		using F = Unit<TagT, RatioOp<typemap::get<Map1, TagT, std::ratio<0>>, typemap::get<Map2, TagT, std::ratio<0>>>>;

		using Result = simplify<normalize<from_list<typelist::map<Tags, F>>>>;
	};

	template<typename T, typename R>
	struct Pow
	{
		using Us1 = to_unit_set<T>;

		template<typename U>
		using F = Unit<unit_tag<U>, std::ratio_multiply<unit_pow<U>, R>>;

		using List1 = to_list<Us1>;
		using List2 = typelist::map<List1, F>;

		using Result = simplify<normalize<from_list<List2>>>;
	};

	template<typename T>
	struct IsUnit : public std::false_type {};

	template<typename TagT, typename PowT>
	struct IsUnit<Unit<TagT, PowT>> : public std::true_type {};

	template<typename T>
	struct IsUnitSet : public std::false_type {};

	template<typename... Units>
	struct IsUnitSet<UnitSet<Units...>> : public std::true_type {};

} // namespace detail
} // namespace traits
} // namespace rafalw

#endif /* RAFALW_UNITS_HPP_ */
