#ifndef XTRAS_TYPEMATH_HPP_
#define XTRAS_TYPEMATH_HPP_

#include <type_traits>
#include <functional>

namespace xtras {
namespace typemath {

template<auto V>
using make_integral_constant = std::integral_constant<decltype(V), V>;

template<typename Op, typename T1, typename T2>
using binary_op = make_integral_constant<Op{}(T1::value, T2::value)>;

template<typename Op, typename T1>
using unary_op = make_integral_constant<Op{}(T1::value)>;


template<typename T1, typename T2>
using plus = binary_op<std::plus<>, T1, T2>;

template<typename T1, typename T2>
using minus = binary_op<std::minus<>, T1, T2>;

template<typename T1, typename T2>
using multiplies = binary_op<std::multiplies<>, T1, T2>;

template<typename T1, typename T2>
using divides = binary_op<std::divides<>, T1, T2>;

template<typename T1, typename T2>
using modulus = binary_op<std::modulus<>, T1, T2>;

template<typename T1>
using negate = unary_op<std::negate<>, T1>;


template<typename T1, typename T2>
using equal_to = binary_op<std::equal_to<>, T1, T2>;

template<typename T1, typename T2>
using not_equal_to = binary_op<std::not_equal_to<>, T1, T2>;

template<typename T1, typename T2>
using greater = binary_op<std::greater<>, T1, T2>;

template<typename T1, typename T2>
using greater_equal = binary_op<std::greater_equal<>, T1, T2>;

template<typename T1, typename T2>
using less = binary_op<std::less<>, T1, T2>;

template<typename T1, typename T2>
using less_equal = binary_op<std::less_equal<>, T1, T2>;


template<typename T>
using logical_not = unary_op<std::logical_not<>, T>;

template<typename T1, typename T2>
using logical_or = binary_op<std::logical_or<>, T1, T2>;

template<typename T1, typename T2>
using logical_and = binary_op<std::logical_and<>, T1, T2>;


template<typename T>
using bit_not = unary_op<std::bit_not<>, T>;

template<typename T1, typename T2>
using bit_or = binary_op<std::bit_or<>, T1, T2>;

template<typename T1, typename T2>
using bit_and = binary_op<std::bit_and<>, T1, T2>;

template<typename T1, typename T2>
using bit_xor = binary_op<std::bit_xor<>, T1, T2>;

} // namespace typemath
} // namespace xtras

#endif // XTRAS_TYPEMATH_HPP_
