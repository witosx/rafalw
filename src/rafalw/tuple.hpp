#ifndef RAFALW_TUPLE_HPP_
#define RAFALW_TUPLE_HPP_

#include <rafalw/utils/static.hpp>
#include <functional>
#include <utility>
#include <tuple>

inline namespace rafalw {
namespace tuple {

namespace detail {

    template<typename FunctionT, typename TupleT, std::size_t... I>
    constexpr decltype(auto) apply_impl(FunctionT&& function, TupleT&& tuple, std::index_sequence<I...>)
    {
        return std::invoke(std::forward<FunctionT>(function), std::get<I>(std::forward<TupleT>(tuple))...);
    }

} // namespace detail

template<typename TupleT, typename FunctionT>
constexpr decltype(auto) apply(FunctionT&& function, TupleT&& tuple)
{
    return detail::apply_impl(
        std::forward<FunctionT>(function),
        std::forward<TupleT>(tuple),
        std::make_index_sequence<std::tuple_size<std::decay_t<TupleT>>::value>{}
    );
}

template<typename RetT, typename TupleT, typename FunctionT>
constexpr decltype(auto) element_apply(FunctionT&& function, TupleT&& tuple, std::size_t index)
{
    return utils::static_integral<RetT, std::size_t, 0, std::tuple_size<std::decay_t<TupleT>>::value>(index, [&](auto ic){
        return function(std::get<ic()>(tuple));
    });
}

} // namespace tuple
} // namespace rafalw

#endif // RAFALW_TUPLE_HPP_
