#ifndef RAFALW_STRINGS_PARSE_HPP_
#define RAFALW_STRINGS_PARSE_HPP_

#include <rafalw/utils/Error.hpp>
#include <rafalw/utils/demangle.hpp>
#include <rafalw/utils/Type.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/utility/string_view.hpp>

inline namespace rafalw {
namespace strings {

class ParseError : public utils::Error
{
public:
    template<typename T>
    ParseError(const boost::string_view& str, utils::Type<T>) :
        Error{ "can't parse '", str, "' as ", utils::demangle<T>() }
    {}
};

template<typename T>
auto parse(const boost::string_view& str) -> T
{
    try {
        return boost::lexical_cast<T>(str.data(), str.length());
    } catch (const boost::bad_lexical_cast&) {
        throw ParseError{ str, utils::Type<T>{} };
    }
}

} // namespace strings
} // namespace rafalw

#endif // RAFALW_STRINGS_PARSE_HPP_
