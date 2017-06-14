#ifndef RAFALW_CSV_COLUMNPARSEERROR_HPP_
#define RAFALW_CSV_COLUMNPARSEERROR_HPP_

#include <rafalw/csv/ColumnError.hpp>
#include <rafalw/utils/helpers.hpp>
#include <boost/core/demangle.hpp>

inline namespace rafalw {
namespace csv {

class ColumnParseErrorBase : public ColumnError
{
protected:
    template<typename ValueT, typename... Args>
    ColumnParseErrorBase(const Context& ctx, int index, const std::string_view& str, utils::WrappedType<ValueT>, const Args&... args) :
		ColumnError{ ctx, index, "parse error - string '", str, "' can't be converted to ", boost::core::demangle(typeid(ValueT).name()), " (",  args..., ")" }
    {}
};

template<typename ValueT>
class ColumnParseError : public ColumnParseErrorBase
{
public:
    template<typename... Args>
    ColumnParseError(const Context& ctx, int index, const std::string_view& str, const Args&... args) :
		ColumnParseErrorBase{ ctx, index, str, utils::wrap<ValueT>(), args... }
    {}
};

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_COLUMNPARSEERROR_HPP_
