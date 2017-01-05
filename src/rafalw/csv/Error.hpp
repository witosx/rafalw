#ifndef RAFALW_CSV_ERROR_HPP_
#define RAFALW_CSV_ERROR_HPP_

#include <rafalw/utils/Error.hpp>

inline namespace rafalw {
namespace csv {

class Error : public utils::Error
{
public:
    template<typename... Args>
    Error(const Args&... args) :
        utils::Error{ "csv error: ", args... }
    {}
};

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_ERROR_HPP_
