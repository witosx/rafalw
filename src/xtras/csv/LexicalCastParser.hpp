#ifndef XTRAS_CSV_LEXICALCASTPARSER_HPP_
#define XTRAS_CSV_LEXICALCASTPARSER_HPP_

#include <boost/lexical_cast.hpp>
#include <string_view>

namespace xtras {
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
} // namespace xtras

#endif // XTRAS_CSV_LEXICALCASTPARSER_HPP_
