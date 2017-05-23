#ifndef RAFALW_LOGGING_HPP_
#define RAFALW_LOGGING_HPP_

#include <iostream>
#include <iomanip>

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

inline auto to_chars(const Level level) -> const char*
{
    switch (level)
    {
    case Level::DEBUG:
        return "debug";
    case Level::INFO:
        return "info";
    case Level::NOTICE:
        return "notice";
    case Level::WARN:
        return "warn";
    case Level::ERROR:
        return "error";
    case Level::FATAL:
        return "fatal";
    }

    return "";
}

template<typename... Args>
auto log(const Level level, const Args&... args) -> void
{
    std::cerr << to_chars(level) << " " << std::fixed;
    (std::cerr << ... << args) << "\n";
}

template<typename... Args>
auto debug(const Args&... args) -> void
{
    log(Level::DEBUG, args...);
}

template<typename... Args>
auto info(const Args&... args) -> void
{
    log(Level::INFO, args...);
}

template<typename... Args>
auto error(const Args&... args) -> void
{
    log(Level::ERROR, args...);
}

} // namespace logging
} // namespace rafalw

#endif // RAFALW_LOGGING_HPP_
