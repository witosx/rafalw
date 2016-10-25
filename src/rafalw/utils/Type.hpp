#ifndef RAFALW_UTILS_TYPE_HPP_
#define RAFALW_UTILS_TYPE_HPP_

inline namespace rafalw {
namespace utils {

template<typename T>
struct Type
{
    using type = T;
};

template<typename T>
static constexpr auto type = Type<T>{};

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_TYPE_HPP_
