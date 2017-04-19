#ifndef RAFALW_LOGGING_HPP_
#define RAFALW_LOGGING_HPP_

#include <iostream>

inline namespace rafalw {
namespace logging {

enum class Level
{
    DEBUG,
    INFO,
    NOTICE,
    WARN,
    ERROR,
    FATAL
};

template<typename... Args>
auto log(const Level level, const Args&... args) -> void
{
    (std::cerr << ... << args) << "\n";
}

template<typename... Args>
auto debug(const Args&... args) -> void
{
    log(Level::DEBUG, args...);
}

template<typename... Args>
auto error(const Args&... args) -> void
{
    log(Level::ERROR, args...);
}

} // namespace logging
} // namespace rafalw

#endif // RAFALW_LOGGING_HPP_
