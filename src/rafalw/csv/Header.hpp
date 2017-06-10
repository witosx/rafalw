#ifndef RAFALW_CSV_HEADER_HPP_
#define RAFALW_CSV_HEADER_HPP_

#include <rafalw/csv/StreamReader.hpp>
#include <rafalw/csv/ColumnParsed.hpp>
#include <rafalw/csv/ColumnSimple.hpp>
#include <rafalw/csv/Error.hpp>
#include <rafalw/utils/ScopeGuard.hpp>
#include <boost/container/small_vector.hpp>
#include <unordered_map>

inline namespace rafalw {
namespace csv {

class BaseHeader
{
public:
    class UnknownColumnError : public Error
    {
    public:
        UnknownColumnError(const Context& context, const std::string_view& name) :
            Error{ context, "unknown column '", name, "'" }
        {}
    };

    explicit BaseHeader(const Context& context) :
        m_context{ context }
    {}

    auto context() const -> const Context&
	{
    	return m_context;
	}

private:
    Context m_context;
};

template<typename CharT>
class BasicHeaderColumn
{
public:
	using Char = CharT;
	using String = std::basic_string<Char>;
	using StringView = std::basic_string_view<Char>;

	BasicHeaderColumn(int index, const StringView& name) :
		m_index{ index },
		m_name{ name }
	{}

	auto index() const -> int
	{
		return m_index;
	}

	auto name() const -> const String&
	{
		return m_name;
	}

private:
	int m_index;
	String m_name;
};

template<typename CharT>
class BasicHeader : public BaseHeader
{
public:
    using Char = CharT;
    using String = std::basic_string<Char>;
    using StringView = std::basic_string_view<Char>;

    using Row = BasicRow<Char>;
    using Column = BasicHeaderColumn<Char>;

    using Columns = boost::container::small_vector<Column, 32>;

    explicit BasicHeader(const Row& header_row) :
		BaseHeader{ header_row.context() }
    {
        for (auto&& e: header_row.columns())
        {
            m_data.emplace_back(m_map.size(), e);
            m_map.emplace(m_data.back().name(), &m_data.back());
        }
    }

    auto column(const StringView& name) const -> const Column&
    {
        try {
            return *m_map.at(name);
        } catch (const std::out_of_range&) {
            throw UnknownColumnError{ context(), name };
        }
    }

    auto columns() const -> const Columns&
    {
        return m_data;
    }

private:
    Columns m_data;
    std::unordered_map<std::string_view, Column*> m_map;
};

template<typename CharT>
auto header(const BasicRow<CharT>& header) -> BasicHeader<CharT>
{
    return BasicHeader<CharT>{ header };
}

template<typename ReaderT>
auto header(ReaderT& reader) -> decltype(header(peek(reader)))
{
    const auto at_exit = utils::scope_guard([&]{
        update(reader);
    });

    return header(peek(reader));
}

template<typename CharT>
auto column(const BasicHeader<CharT>& header, typename BasicHeader<CharT>::StringView column_name) -> decltype(column(header.column(column_name).index()))
{
    return column(header.column(column_name).index());
}

template<typename CharT, typename ValueT, bool OPTIONAL_V>
auto column(const BasicHeader<CharT>& header, typename BasicHeader<CharT>::StringView column_name, ColumnParsedTag<ValueT, OPTIONAL_V> tag) -> decltype(column(header.column(column_name).index(), tag))
{
    return column(header.column(column_name).index(), tag);
}

template<typename CharT>
auto begin(const BasicHeader<CharT>& header) -> decltype(header.columns().begin())
{
    return header.columns().begin();
}

template<typename CharT>
auto end(const BasicHeader<CharT>& header) -> decltype(header.columns().end())
{
    return header.columns().end();
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_HEADER_HPP_
