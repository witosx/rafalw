#ifndef RAFALW_CSV_COLUMNPARSED_HPP_
#define RAFALW_CSV_COLUMNPARSED_HPP_

#include <rafalw/csv/Row.hpp>
#include <rafalw/csv/ColumnSimple.hpp>
#include <rafalw/csv/ColumnError.hpp>
#include <rafalw/utils/helpers.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/core/demangle.hpp>

inline namespace rafalw {
namespace csv {

class ColumnParsedBase
{
public:
	explicit ColumnParsedBase(int index) :
		m_index{ index }
	{}

	ColumnParsedBase(const ColumnSimple column) :
	    ColumnParsedBase{ column.index() }
	{}

    auto index() const -> int
    {
        return m_index;
    }

private:
    int m_index;
};

class ParserError : public BaseError
{
public:
    template<typename ValueT>
    ParserError(const std::string_view& str, utils::Type<ValueT>) :
        BaseError{ "parser error - string '", str, "' can't be converted to ", boost::core::demangle(typeid(ValueT).name()) }
    {}
};

template<typename ValueT>
struct LexicalCastParser
{
    auto operator ()(const std::string_view& str) const -> ValueT
    {
        try {
            return boost::lexical_cast<ValueT>(str);
        }
        catch (const boost::bad_lexical_cast&) {
            throw ParserError{ str, utils::type<ValueT> };
        }
    }
};

template<typename ValueT>
using DefaultParser = LexicalCastParser<ValueT>;

template<typename ValueT, typename ParserT, bool OPTIONAL_V>
class ColumnParsedBasic : public ColumnParsedBase
{
public:
	using ColumnParsedBase::ColumnParsedBase;
	using Value = ValueT;
	using Parser = ParserT;

	auto parser() const -> const Parser&
    {
	    return m_parser;
    }

private:
	Parser m_parser;
};

template<typename ValueT>
using ColumnParsed = ColumnParsedBasic<ValueT, DefaultParser<ValueT>, false>;

template<typename ValueT>
using ColumnParsedOptional = ColumnParsedBasic<ValueT, DefaultParser<ValueT>, true>;

template<typename ValueT, bool OPTIONAL_V>
struct ColumnParsedTag {};

template<typename ValueT>
static constexpr auto as = ColumnParsedTag<ValueT, false>{};

template<typename ValueT>
static constexpr auto as_optional = ColumnParsedTag<ValueT, true>{};

template<typename ValueT, bool OPTIONAL_V>
inline auto column(int index, ColumnParsedTag<ValueT, OPTIONAL_V>) -> ColumnParsedBasic<ValueT, DefaultParser<ValueT>, OPTIONAL_V>
{
    return ColumnParsedBasic<ValueT, DefaultParser<ValueT>, OPTIONAL_V>{ index };
}

template<typename CharT, typename ValueT, typename ParserT, bool OPTIONAL_V>
auto fetch(const BasicRow<CharT>& row, const ColumnParsedBasic<ValueT, ParserT, OPTIONAL_V>& column) -> std::conditional_t<OPTIONAL_V, std::optional<ValueT>, ValueT>
{
    const auto str = fetch(row, column.index());

    if constexpr (OPTIONAL_V)
    {
        if (str.empty())
            return std::nullopt;
    }

    try {
        return column.parser()(str);
    } catch (const ParserError& e) {
        throw ColumnError{ row.context(), column.index(), e.what() };
    }
}

template<typename CharT, typename ValueT, bool OPTIONAL_V>
auto fetch(const BasicRow<CharT>& row, int index, ColumnParsedTag<ValueT, OPTIONAL_V> tag) -> decltype(fetch(row, column(index, tag)))
{
	return fetch(row, column(index, tag));
}

} // namespace csv
} // namespace rafalw

#endif /* RAFALW_CSV_COLUMNPARSED_HPP_ */
