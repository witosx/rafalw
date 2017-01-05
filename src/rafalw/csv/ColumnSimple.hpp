#ifndef RAFALW_CSV_COLUMNSIMPLE_HPP_
#define RAFALW_CSV_COLUMNSIMPLE_HPP_

#include <rafalw/csv/Column.hpp>
#include <rafalw/csv/Line.hpp>

inline namespace rafalw {
namespace csv {

class ColumnSimple
{
public:
    ColumnSimple(const Column& c) :
        m_column{ c }
    {}

    auto name() const -> const std::string&
    {
        return m_column.name();
    }

    auto index() const -> int
    {
        return m_column.index();
    }

private:
    const Column& m_column;
};

inline auto fetch(const Line& line, ColumnSimple column) -> const Line::StringView&
{
    return line.get(column.index());
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_COLUMNSIMPLE_HPP_
