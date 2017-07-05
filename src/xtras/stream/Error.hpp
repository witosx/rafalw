#ifndef XTRAS_STREAM_ERROR_HPP_
#define XTRAS_STREAM_ERROR_HPP_

#include <xtras/utils/Error.hpp>

namespace xtras {
namespace stream {

class Error : public utils::Error
{
public:
    template<typename... Args>
    explicit Error(const Args&... args) :
        utils::Error{ "xtras::stream ", args... }
    {}
};

} // namespace stream
} // namespace xtras

#endif // XTRAS_STREAM_ERROR_HPP_
