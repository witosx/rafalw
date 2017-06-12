#ifndef RAFALW_STREAM_ERROR_HPP_
#define RAFALW_STREAM_ERROR_HPP_

#include <rafalw/utils/Error.hpp>

inline namespace rafalw {
namespace stream {

class Error : public utils::Error
{
public:
    template<typename... Args>
    explicit Error(const Args&... args) :
        utils::Error{ "rafalw::stream ", args... }
    {}
};

} // namespace stream
} // namespace rafalw

#endif // RAFALW_STREAM_ERROR_HPP_
