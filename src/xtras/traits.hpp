#ifndef XTRAS_TRAITS_HPP_
#define XTRAS_TRAITS_HPP_

#include <type_traits>

namespace xtras {
namespace traits {

template<typename T>
using size = std::integral_constant<decltype(sizeof(T)), sizeof(T)>;

template<typename T>
using type_member = typename T::type;

} // namespace traits
} // namespace xtras

#endif // XTRAS_TRAITS_HPP_
