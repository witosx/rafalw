#ifndef RAFALW_UTILS_ERROR_HPP_
#define RAFALW_UTILS_ERROR_HPP_

#include <rafalw/strings/misc.hpp>
#include <exception>
#include <string>

inline namespace rafalw {
namespace utils {

class Error : public std::exception
{
public:
    template<typename... Args>
    Error(const Args&... args) :
        m_message{ strings::stringify(args...) }
    {}

    auto what() const noexcept -> const char* final override
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_ERROR_HPP_
