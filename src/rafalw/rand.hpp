#ifndef RAFALW_RAND_HPP_
#define RAFALW_RAND_HPP_

#include <functional>
#include <random>
#include <type_traits>

inline namespace rafalw {

namespace rand {

namespace detail {

    template<typename T, typename... Args>
    auto create(Args&&... args)
    {
        auto reng = std::default_random_engine{ std::random_device{}() };
        auto rdis = T{ std::forward<Args>(args)... };
        return std::bind(std::move(rdis), std::move(reng));
    }

} // namespace detail

template<typename T = int>
auto uniform_int(T a = 0, T b = std::numeric_limits<T>::max())
{
    return detail::create<std::uniform_int_distribution<T>>(a, b);
}

template<typename T = double>
auto uniform_real(T a = 0.0, T b = 1.0)
{
    return detail::create<std::uniform_real_distribution<T>>(a, b);
}

template<typename T, std::enable_if_t<std::is_integral<T>::value>* = nullptr>
auto uniform(T a, T b)
{
    return uniform_int(a, b);
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value>* = nullptr>
auto uniform(T a, T b)
{
    return uniform_real(a, b);
}

template<typename T>
auto normal(T mean = 0.0, T stddev = 1.0)
{    
    return detail::create<std::normal_distribution<T>>(mean, stddev);
}

} // namespace rand
} // namespace rafalw

#endif // RAFALW_RAND_HPP_
