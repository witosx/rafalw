#ifndef XTRAS_CSV_DEFAULTPARSER_HPP_
#define XTRAS_CSV_DEFAULTPARSER_HPP_

#include <xtras/csv/LexicalCastParser.hpp>

namespace xtras {
namespace csv {

template<typename ValueT>
using DefaultParser = LexicalCastParser<ValueT>;

} // namespace csv
} // namespace xtras

#endif // XTRAS_CSV_DEFAULTPARSER_HPP_
