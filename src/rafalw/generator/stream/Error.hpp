#ifndef RAFALW_GENERATOR_STREAM_ERROR_HPP_
#define RAFALW_GENERATOR_STREAM_ERROR_HPP_

#include <rafalw/utils/Error.hpp>

inline namespace rafalw {
namespace generator {
namespace stream {

class Error : public utils::Error
{
public:
    template<typename... Args>
    explicit Error(const Args&... args) :
        utils::Error{ "generator::stream ", args... }
    {}
};

} // namespace stream
} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_STREAM_ERROR_HPP_
