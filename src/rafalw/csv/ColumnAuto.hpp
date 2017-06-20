#ifndef RAFALW_CSV_COLUMNAUTO_HPP_
#define RAFALW_CSV_COLUMNAUTO_HPP_

#include <rafalw/csv/Row.hpp>
#include <rafalw/csv/ColumnSimple.hpp>
#include <rafalw/csv/ColumnError.hpp>
#include <rafalw/csv/ColumnParseError.hpp>
#include <rafalw/csv/DefaultParser.hpp>
#include <optional>

inline namespace rafalw {
namespace csv {

class ColumnAuto
{
public:
    explicit ColumnAuto(int index) :
        m_index{ index }
    {}

    ColumnAuto(const ColumnSimple column) :
        ColumnAuto{ column.index() }
    {}

    auto index() const -> int
    {
        return m_index;
    }

private:
    int m_index;
};

template<typename CharT>
class ColumnAutoProxy
{
public:
    using Char = CharT;
    using StringView = std::basic_string_view<CharT>;

    ColumnAutoProxy(const BasicRow<CharT>& row, const ColumnAuto column) :
        m_row{ row },
        m_column{ column }
    {}

    ColumnAutoProxy(const ColumnAutoProxy&) = delete;
    ColumnAutoProxy(ColumnAutoProxy&&) = delete;

    template<typename T>
    operator T() const &&
    {
        const auto str = fetch(m_row, m_column.index());

        return parse<T>(str);
    }

    template<typename T>
    operator std::optional<T>() const &&
    {
        const auto str = fetch(m_row, m_column.index());

        if (str.empty())
            return {};

        return parse<T>(str);
    }

private:
    const BasicRow<CharT>& m_row;
    ColumnAuto m_column;

    template<typename T>
    auto parse(const StringView string) const -> T
    {
        auto parser = DefaultParser<T>{};

        try {
            return parser(string);
        } catch (const std::exception& e) {
            throw ColumnParseError<T>{ m_row.context(), m_column.index(), string, e.what() };
        }
    }
};

struct ColumnAutoTag {};

inline constexpr auto as_auto = ColumnAutoTag{};


inline auto column(int index, ColumnAutoTag) -> ColumnAuto
{
    return ColumnAuto{ index };
}

template<typename CharT>
auto fetch(const BasicRow<CharT>& row, const ColumnAuto column) -> ColumnAutoProxy<CharT>
{
    return ColumnAutoProxy<CharT>{ row, column };
}

template<typename CharT>
auto fetch(const BasicRow<CharT>& row, int index, ColumnAutoTag tag) -> decltype(fetch(row, column(index, tag)))
{
    return fetch(row, column(index, tag));
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_COLUMNAUTO_HPP_
