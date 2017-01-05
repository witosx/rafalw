#ifndef RAFALW_CSV_COLUMNTYPED_HPP_
#define RAFALW_CSV_COLUMNTYPED_HPP_

#include <rafalw/csv/Column.hpp>
#include <rafalw/csv/Line.hpp>

inline namespace rafalw {
namespace csv {

template<typename T>
class ColumnTyped
{
public:
    ColumnTyped(const Column& c) :
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

template<typename T>
inline auto fetch(const Line& line, ColumnTyped<T> column) -> T
{
    return line.get<T>(column.index());
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_COLUMNTYPED_HPP_
