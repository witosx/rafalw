#ifndef XTRAS_UTILS_DEMANGLE_HPP_
#define XTRAS_UTILS_DEMANGLE_HPP_

#include <boost/core/demangle.hpp>
#include <string>

namespace xtras {
inline namespace utils {

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
} // namespace xtras

#endif // XTRAS_UTILS_DEMANGLE_HPP_
