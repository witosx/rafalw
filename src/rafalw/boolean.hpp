#ifndef RAFALW_BOOLEAN_HPP_
#define RAFALW_BOOLEAN_HPP_

inline namespace rafalw {
namespace boolean {

inline auto any() -> bool
{
    return false;
}

template<typename T, typename... Args>
auto any(const T& arg0, const Args&... args) -> bool
{
    if (static_cast<bool>(arg0))
        return true;

    return any(args...);
}

}
}

#endif // RAFALW_BOOLEAN_HPP_
