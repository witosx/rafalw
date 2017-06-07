#ifndef RAFALW_CSV_ROW_HPP_
#define RAFALW_CSV_ROW_HPP_

#include <rafalw/csv/Context.hpp>
#include <rafalw/csv/Error.hpp>
#include <rafalw/generator/string/Tokens.hpp>
#include <rafalw/strings/parse.hpp>
#include <rafalw/utils/demangle.hpp>
#include <string>
#include <sstream>
#include <vector>

inline namespace rafalw {
namespace csv {

class BaseRow
{
public:
    enum class EmptyPolicy
    {
        KEEP,
        IGNORE
    };

    class ColumnIndexOutOfRangeError : public Error
    {
    public:
        ColumnIndexOutOfRangeError(const Context& context, int column_index) :
            Error{ context, "column index (", column_index, ") out of range" },
            m_columnIndex{ column_index }
        {}

        auto columnIndex() const -> int
        {
            return m_columnIndex;
        }

    private:
        int m_columnIndex;
    };

    using ColumnIndex = int;
};

template<typename CharT>
class BasicRow : public BaseRow
{
public:
    using Char = CharT;
    using String = std::basic_string<Char>;
    using StringView = std::basic_string_view<Char>;

    BasicRow(const Context& context, StringView delimiters, EmptyPolicy policy) :
        m_context{ context },
        m_delimiters{ delimiters },
        m_policy{ policy }
    {}

    auto context() const -> const Context&
    {
        return m_context;
    }

    auto parse(StringView line) -> void
    {
        m_columns.clear();

        for (auto&& token: generator::string::tokens(line, m_delimiters))
        {
            if (token.empty() && m_policy == EmptyPolicy::IGNORE)
                continue;

            m_columns.push_back(token);
        }
    }

    auto column(ColumnIndex index) const -> const StringView&
    {
        try {
            return m_columns.at(index);
        } catch (const std::out_of_range&) {
            throw ColumnIndexOutOfRangeError{ m_context, index };
        }
    }

    auto columns() const -> const std::vector<StringView>&
    {
        return m_columns;
    }

private:
    const Context& m_context;
    String m_delimiters;
    EmptyPolicy m_policy;

    std::vector<StringView> m_columns;
};

template<typename CharT>
auto fetch(const BasicRow<CharT>& row, typename BasicRow<CharT>::ColumnIndex index) -> decltype(row.column(index))
{
    return row.column(index);
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_ROW_HPP_
