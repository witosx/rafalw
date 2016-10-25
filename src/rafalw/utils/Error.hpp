#ifndef RAFALW_UTILS_ERROR_HPP_
#define RAFALW_UTILS_ERROR_HPP_

#include <rafalw/utils/strings.hpp>
#include <exception>
#include <string>

inline namespace rafalw {
namespace utils {

class Error : public std::exception
{
private:
    std::string m_message;

public:
    template<typename... Args>
    Error(const Args&... args) :
        m_message{ stringify(args...) }
    {}

    auto what() const noexcept -> const char*
    {
        return m_message.c_str();
    }
};

} // namespace utils
} // namespace rafalw

#endif // RAFALW_UTILS_ERROR_HPP_
