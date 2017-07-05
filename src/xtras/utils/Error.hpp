#ifndef XTRAS_UTILS_ERROR_HPP_
#define XTRAS_UTILS_ERROR_HPP_

#include <exception>
#include <string>
#include <sstream>

namespace xtras {
inline namespace utils {

class Error : public std::exception
{
public:
    template<typename... Args>
    explicit Error(const Args&... args) :
        m_message{ createMessage(args...) }
    {}

    auto what() const noexcept -> const char* final override
    {
        return m_message.c_str();
    }

private:
    std::string m_message;

    template<typename... Args>
    static auto createMessage(const Args&... args) -> std::string
    {
        auto ss = std::ostringstream{};
        (ss << ... << args);
        return ss.str();
    }
};

} // namespace utils
} // namespace xtras

#endif // XTRAS_UTILS_ERROR_HPP_
