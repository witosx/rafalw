#ifndef RAFALW_CSV_PARSEDCOLUMN_HPP_
#define RAFALW_CSV_PARSEDCOLUMN_HPP_

#include <rafalw/csv/Column.hpp>
#include <rafalw/csv/Row.hpp>

inline namespace rafalw {
namespace csv {

template<typename ValueT, bool OptionalV>
struct Parsed {};

template<typename ValueT>
static constexpr auto parsed = Parsed<ValueT, false>{};

template<typename ValueT>
static constexpr auto parsed_opt = Parsed<ValueT, true>{};


template<typename ValueT, typename CharT, bool OptionalV>
class BasicParsedColumn
{
public:
    using Char = CharT;
    using Value = ValueT;
    using Column = BasicColumn<Char>;

    explicit BasicParsedColumn(const Column& c) :
        m_column{ c }
    {}

    auto column() const -> const Column&
    {
        return m_column;
    }

private:
    Column m_column;
};

template<typename IndexerT, typename CharT, typename ValueT, bool OptionalV>
auto column(const IndexerT& indexer, const CharT* name, Parsed<ValueT, OptionalV>) -> BasicParsedColumn<ValueT, CharT, OptionalV>
{
    return BasicParsedColumn<ValueT, CharT, OptionalV>{ column(indexer, name) };
}

template<typename CharT, typename ValueT, bool OptionalTV>
auto fetch(const BasicRow<CharT>& row, const BasicParsedColumn<ValueT, CharT, OptionalTV>& parsed) -> std::conditional_t<OptionalTV, std::optional<ValueT>, ValueT>
{
    const auto str = fetch(row, parsed.column());

    if constexpr (OptionalTV)
    {
        if (str.empty())
            return {};
    }

    try {
        return boost::lexical_cast<ValueT>(str);
    } catch (const boost::bad_lexical_cast&) {
        throw ColumnError{ row.context(), parsed.column(), "value '", str, "' can't be parsed as ", boost::core::demangle(typeid(ValueT).name()) };
    }
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_PARSEDCOLUMN_HPP_
