#ifndef XTRAS_CSV_COLUMNPARSEERROR_HPP_
#define XTRAS_CSV_COLUMNPARSEERROR_HPP_

#include <xtras/csv/ColumnError.hpp>
#include <xtras/utils/helpers.hpp>
#include <boost/core/demangle.hpp>

namespace xtras {
namespace csv {

class ColumnParseErrorBase : public ColumnError
{
protected:
    template<typename ValueT, typename... Args>
    ColumnParseErrorBase(const Context& ctx, int index, const std::string_view& str, WrappedType<ValueT>, const Args&... args) :
		ColumnError{ ctx, index, "parse error - string '", str, "' can't be converted to ", boost::core::demangle(typeid(ValueT).name()), " (",  args..., ")" }
    {}
};

template<typename ValueT>
class ColumnParseError : public ColumnParseErrorBase
{
public:
    template<typename... Args>
    ColumnParseError(const Context& ctx, int index, const std::string_view& str, const Args&... args) :
		ColumnParseErrorBase{ ctx, index, str, wrap<ValueT>(), args... }
    {}
};

} // namespace csv
} // namespace xtras

#endif // XTRAS_CSV_COLUMNPARSEERROR_HPP_
