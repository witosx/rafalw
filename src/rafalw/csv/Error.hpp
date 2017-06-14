#ifndef RAFALW_CSV_ERROR_HPP_
#define RAFALW_CSV_ERROR_HPP_

#include <rafalw/utils/Error.hpp>

inline namespace rafalw {
namespace csv {

class Error : public utils::Error
{
    using utils::Error::Error;
};

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_ERROR_HPP_
