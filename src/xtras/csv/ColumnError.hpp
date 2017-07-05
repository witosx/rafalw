#ifndef XTRAS_CSV_COLUMNERROR_HPP_
#define XTRAS_CSV_COLUMNERROR_HPP_

#include <xtras/csv/SourceError.hpp>

namespace xtras {
namespace csv {

class ColumnError : public SourceError
{
public:
    template<typename... Args>
    ColumnError(const Context& context, int index, const Args&... args) :
        SourceError{ context, "column:", index, " ", args... }
    {}
};

} // namespace csv
} // namespace xtras

#endif // XTRAS_CSV_COLUMNERROR_HPP_
