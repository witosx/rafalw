#ifndef XTRAS_STRING_PARSE_HPP_
#define XTRAS_STRING_PARSE_HPP_

#include <xtras/utils/Error.hpp>
#include <xtras/utils/demangle.hpp>
#include <xtras/utils/helpers.hpp>
#include <boost/lexical_cast.hpp>
#include <string_view>

namespace xtras {
namespace string {

class ParseError : public Error
{
public:
    template<typename T>
    ParseError(const std::string_view& str, WrappedType<T>) :
        Error{ "can't parse '", str, "' as ", demangle<T>() }
    {}
};

template<typename T>
auto parse(const std::string_view& str) -> T
{
    try {
        return boost::lexical_cast<T>(str.data(), str.length());
    } catch (const boost::bad_lexical_cast&) {
        throw ParseError{ str, WrappedType<T>{} };
    }
}

} // namespace string
} // namespace xtras

#endif // XTRAS_STRING_PARSE_HPP_
