#ifndef RAFALW_UTILS_DEMANGLE_HPP_
#define RAFALW_UTILS_DEMANGLE_HPP_

#include <boost/core/demangle.hpp>
#include <string>

inline namespace rafalw {
namespace utils {

auto demangle(const std::string& name) -> std::string;

template<typename T>
auto demangle() -> std::string;

inline auto demangle(const char* name) -> std::string
{
    return boost::core::demangle(name);
}

template<typename T>
inline auto demangle() -> std::string
{
    return demangle(typeid(T).name());
}

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_DEMANGLE_HPP_
