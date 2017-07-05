#ifndef XTRAS_CSV_ERROR_HPP_
#define XTRAS_CSV_ERROR_HPP_

#include <xtras/utils/Error.hpp>

namespace xtras {
namespace csv {

class Error : public utils::Error
{
    using utils::Error::Error;
};

} // namespace csv
} // namespace xtras

#endif // XTRAS_CSV_ERROR_HPP_
