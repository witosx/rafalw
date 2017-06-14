#ifndef RAFALW_CSV_ROW_HPP_
#define RAFALW_CSV_ROW_HPP_

#include <rafalw/csv/ColumnError.hpp>
#include <rafalw/csv/Context.hpp>
#include <rafalw/string/Tokens.hpp>
#include <string>
#include <string_view>
#include <vector>

inline namespace rafalw {
namespace csv {

enum class empty_fields
{
    keep,
    drop
};

class BaseRow
{
public:
    auto context() const -> const Context&
    {
        return m_context;
    }

protected:
    BaseRow(const Context& context, empty_fields empty) :
        m_context{ context },
        m_keepEmptyTokens{ empty == empty_fields::keep }
    {}

    auto keepEmptyTokens() const -> bool
    {
        return m_keepEmptyTokens;
    }

private:
    const Context& m_context;
    bool m_keepEmptyTokens;
};

template<typename CharT>
class BasicRow : public BaseRow
{
public:
    using Char = CharT;
    using String = std::basic_string<Char>;
    using StringView = std::basic_string_view<Char>;

    using Columns = std::vector<StringView>;

    BasicRow(const Context& context, StringView delimiters, empty_fields keep_empty) :
        BaseRow{ context, keep_empty },
        m_delimiters{ delimiters }
    {}

    BasicRow(const BasicRow&) = delete;

    auto parse(StringView line) -> void
    {
        m_columns.clear();
        for (auto&& token: string::tokens(line, m_delimiters))
            if (!token.empty() || keepEmptyTokens())
                m_columns.push_back(token);
    }

    auto columns() const -> const Columns&
    {
        return m_columns;
    }

private:
    String m_delimiters;
    Columns m_columns;
};

using Row = BasicRow<char>;

template<typename CharT>
auto fetch(const BasicRow<CharT>& row, int index) -> decltype(row.columns().at(index))
{
    try {
        return row.columns().at(index);
    } catch (const std::out_of_range&) {
        throw ColumnError{ row.context(), index, "not found" };
    }
}

template<typename CharT>
auto begin(const BasicRow<CharT>& row) -> decltype(row.columns().begin())
{
    return row.columns().begin();
}

template<typename CharT>
auto end(const BasicRow<CharT>& row) -> decltype(row.columns().end())
{
    return row.columns().end();
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_ROW_HPP_
