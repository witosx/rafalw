#ifndef RAFALW_CSV_COLUMNERROR_HPP_
#define RAFALW_CSV_COLUMNERROR_HPP_

#include <rafalw/csv/SourceError.hpp>

inline namespace rafalw {
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
} // namespace rafalw

#endif // RAFALW_CSV_COLUMNERROR_HPP_
