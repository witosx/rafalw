#ifndef RAFALW_UTILS_IGNORE_HPP_
#define RAFALW_UTILS_IGNORE_HPP_

inline namespace rafalw {
namespace utils {

struct Ignore
{
    template<typename T>
    Ignore(const T&) {}

    template<typename T>
    auto operator =(const T&) -> Ignore&
    {
        return *this;
    }
};

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_IGNORE_HPP_
