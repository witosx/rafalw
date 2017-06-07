#ifndef RAFALW_CSV_COLUMN_HPP_
#define RAFALW_CSV_COLUMN_HPP_

#include <rafalw/csv/Row.hpp>
#include <string_view>
#include <string>

inline namespace rafalw {
namespace csv {

template<typename CharT>
class BasicColumn
{
public:
    using Char = CharT;
    using String = std::basic_string<Char>;
    using StringView = std::basic_string_view<Char>;

    BasicColumn(int index, StringView name) :
        m_name{ name },
        m_index{ index }
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
auto column(int index, const std::basic_string_view<CharT>& name) -> BasicColumn<CharT>
{
    return BasicColumn<CharT>{ index, name };
}

template<typename CharT>
auto column(int index, const CharT* name) -> BasicColumn<CharT>
{
    return BasicColumn<CharT>{ index, name };
}

using Column = BasicColumn<char>;

class ColumnError : public Error
{
public:
    template<typename... Args>
    ColumnError(const Context& context, const Column& column, const Args&... args) :
        Error{ context, "column '", column.name(), "' ", args... }
    {}
};

template<typename CharT>
auto fetch(const BasicRow<CharT>& row, const BasicColumn<CharT>& column) -> decltype(row.column(column.index()))
{
    try {
        return row.column(column.index());
    } catch (const BaseRow::ColumnIndexOutOfRangeError& e) {
        throw ColumnError{ e.context(), column, "not found" };
    }
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_COLUMN_HPP_
