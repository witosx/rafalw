#ifndef XTRAS_CSV_HEADER_HPP_
#define XTRAS_CSV_HEADER_HPP_

#include <xtras/csv/SourceError.hpp>
#include <xtras/csv/Row.hpp>
#include <xtras/utils/ScopeGuard.hpp>
#include <boost/container/small_vector.hpp>
#include <unordered_map>

namespace xtras {
namespace csv {

class BaseHeader
{
public:
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

class HeaderColumnNotFoundError : public SourceError
{
public:
	HeaderColumnNotFoundError(const Context& context, const std::string_view& name) :
    	SourceError{ context, "header column '", name, "' not found" }
    {}
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
            throw HeaderColumnNotFoundError{ context(), name };
        }
    }

    auto columns() const -> const Columns&
    {
        return m_data;
    }

private:
    Columns m_data;
    std::unordered_map<StringView, Column*> m_map;
};

using Header = BasicHeader<char>;

template<typename CharT>
auto header(const BasicRow<CharT>& header) -> BasicHeader<CharT>
{
    return BasicHeader<CharT>{ header };
}

template<typename ReaderT>
auto header(ReaderT& reader) -> decltype(header(peek(reader)))
{
    const auto at_exit = scope_guard([&]{
        update(reader);
    });

    return header(peek(reader));
}

template<typename CharT, typename... Args>
auto column(const BasicHeader<CharT>& header, typename BasicHeader<CharT>::StringView column_name, Args... args) -> decltype(column(header.column(column_name).index(), std::move(args)...))
{
    return column(header.column(column_name).index(), std::move(args)...);
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
} // namespace xtras

#endif // XTRAS_CSV_HEADER_HPP_
