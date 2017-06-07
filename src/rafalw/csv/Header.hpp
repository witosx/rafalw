#ifndef RAFALW_CSV_HEADER_HPP_
#define RAFALW_CSV_HEADER_HPP_

#include <rafalw/csv/StreamReader.hpp>
#include <rafalw/csv/Column.hpp>
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
};

template<typename CharT>
class BasicHeader : public BaseHeader
{
public:
    using Char = CharT;
    using StringView = std::basic_string_view<Char>;

    using Column = BasicColumn<Char>;
    using Data = boost::container::small_vector<Column, 32>;

    explicit BasicHeader(const BasicRow<Char>& header_row) :
        m_context{ header_row.context() }
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
            throw UnknownColumnError{ m_context, name };
        }
    }

    auto columns() const -> const Data&
    {
        return m_data;
    }

private:
    Context m_context;
    boost::container::small_vector<Column, 32> m_data;
    std::unordered_map<std::string_view, Column*> m_map;
};

template<typename CharT>
auto header(const BasicRow<CharT>& header) -> BasicHeader<CharT>
{
    return BasicHeader<CharT>{ header };
}

template<typename ReaderT>
auto header(ReaderT& reader) -> decltype(header(next(reader)))
{
    const auto at_exit = utils::scope_guard([&reader]{
        update(reader);
    });

    return header(peek(reader));
}

template<typename CharT>
auto column(const BasicHeader<CharT>& header, typename BasicHeader<CharT>::StringView column_name) -> decltype(header.column(column_name))
{
    return header.column(column_name);
}

template<typename CharT>
auto begin(const BasicHeader<CharT>& header) -> typename BasicHeader<CharT>::Data::const_iterator
{
    return header.columns().begin();
}

template<typename CharT>
auto end(const BasicHeader<CharT>& header) -> typename BasicHeader<CharT>::Data::const_iterator
{
    return header.columns().end();
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_HEADER_HPP_
