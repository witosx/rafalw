#ifndef RAFALW_UNITS_HPP_
#define RAFALW_UNITS_HPP_

#include <rafalw/typelist.hpp>
#include <rafalw/typemap.hpp>
#include <ratio>

inline namespace rafalw {
namespace units {

namespace detail {

	template<typename OpT>
	using result = typename OpT::Result;

	template<typename UnitT>
	struct UnitTagOp;

	template<typename UnitT>
	struct UnitPowOp;

	template<typename UnitSetT>
	struct ToListOp;

	template<typename UnitSetT>
	struct ToMapOp;

	template<template<typename, typename> class RatioOp, typename UnitSet1T, typename UnitSet2T>
	struct BinaryOp;

	template<typename Us1, typename R>
	struct PowOp;

} // namespace detail


template<typename Tag, typename Pow = std::ratio<1>>
struct Unit;

template<typename U>
using unit_tag = detail::result<detail::UnitTagOp<U>>;

template<typename U>
using unit_pow = detail::result<detail::UnitPowOp<U>>;


template<typename... U>
struct UnitSet;

template<typename Tag>
using Null = UnitSet<>;

template<typename Tag>
using make = UnitSet<Unit<Tag>>;

template<typename UnitSetT>
using to_list = detail::result<detail::ToListOp<UnitSetT>>;

template<typename UnitSetT>
using to_map = detail::result<detail::ToMapOp<UnitSetT>>;

template<typename Us1, typename Us2>
using mul = detail::result<detail::BinaryOp<std::ratio_add, Us1, Us2>>;

template<typename Us1, typename Us2>
using div = detail::result<detail::BinaryOp<std::ratio_subtract, Us1, Us2>>;

template<typename Us1, typename Pow>
using pow = detail::result<detail::PowOp<Us1, Pow>>;


template<typename Tag, std::intmax_t PowN, std::intmax_t PowD>
struct Unit<Tag, std::ratio<PowN, PowD>> {};

template<typename... Tags, typename... Pows>
struct UnitSet<Unit<Tags, Pows>...> {};


namespace detail {

	template<typename TagT, typename PowT>
	struct UnitTagOp<Unit<TagT, PowT>>
	{
		using Result = TagT;
	};

	template<typename TagT, typename PowT>
	struct UnitPowOp<Unit<TagT, PowT>>
	{
		using Result = PowT;
	};

	template<typename... U>
	struct ToListOp<UnitSet<U...>>
	{
		using Result = typelist::List<U...>;
	};

	template<typename... U>
	struct ToMapOp<UnitSet<U...>>
	{
		using Result = typemap::Map<typemap::Item<unit_tag<U>, unit_pow<U>>...>;
	};

	template<typename L>
	struct FromListOp;

	template<typename... Es>
	struct FromListOp<typelist::List<Es...>>
	{
		using Result = UnitSet<Es...>;
	};

	template<typename L>
	using from_list = result<FromListOp<L>>;

	template<typename Us>
	struct NormalizeOp
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
	using normalize = result<NormalizeOp<Us>>;

	template<template<typename, typename> class RatioOp, typename Us1, typename Us2>
	struct BinaryOp
	{
		using Map1 = to_map<Us1>;
		using Map2 = to_map<Us2>;

		using Tags1 = typemap::keys<Map1>;
		using Tags2 = typemap::keys<Map2>;

		using Tags = typelist::unique<typelist::concat<Tags1, Tags2>>;

		template<typename TagT>
		using F = Unit<TagT, RatioOp<typemap::get<Map1, TagT, std::ratio<0>>, typemap::get<Map2, TagT, std::ratio<0>>>>;

		using Result = normalize<from_list<typelist::map<Tags, F>>>;
	};

	template<typename Us1, typename R>
	struct PowOp
	{
		template<typename U>
		using F = Unit<unit_tag<U>, std::ratio_multiply<unit_pow<U>, R>>;

		using List1 = to_list<Us1>;
		using List2 = typelist::map<List1, F>;

		using Result = normalize<from_list<List2>>;
	};

} // namespace detail
} // namespace traits
} // namespace rafalw

#endif /* RAFALW_UNITS_HPP_ */
