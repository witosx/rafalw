#ifndef RAFALW_CSV_LEXICALCASTPARSER_HPP_
#define RAFALW_CSV_LEXICALCASTPARSER_HPP_

#include <boost/lexical_cast.hpp>
#include <string_view>

inline namespace rafalw {
namespace csv {

template<typename ValueT>
struct LexicalCastParser
{
	template<typename CharT>
    auto operator ()(const std::basic_string_view<CharT>& str) const -> ValueT
    {
		return boost::lexical_cast<ValueT>(str);
    }
};

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_LEXICALCASTPARSER_HPP_
