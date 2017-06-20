#ifndef RAFALW_CSV_DEFAULTPARSER_HPP_
#define RAFALW_CSV_DEFAULTPARSER_HPP_

#include <rafalw/csv/LexicalCastParser.hpp>

inline namespace rafalw {
namespace csv {

template<typename ValueT>
using DefaultParser = LexicalCastParser<ValueT>;

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_DEFAULTPARSER_HPP_
