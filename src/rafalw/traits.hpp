#ifndef RAFALW_TRAITS_HPP_
#define RAFALW_TRAITS_HPP_

#include <type_traits>

inline namespace rafalw {
namespace traits {

template<typename T>
using size = std::integral_constant<decltype(sizeof(T)), sizeof(T)>;

template<typename T>
using type_member = typename T::type;

template<typename T>
using identity = T;

} // namespace traits
} // namespace rafalw

#endif // RAFALW_TRAITS_HPP_
