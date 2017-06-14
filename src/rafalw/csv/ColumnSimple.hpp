#ifndef RAFALW_CSV_COLUMNSIMPLE_HPP_
#define RAFALW_CSV_COLUMNSIMPLE_HPP_

#include <rafalw/csv/ColumnError.hpp>
#include <rafalw/csv/Row.hpp>

inline namespace rafalw {
namespace csv {

class ColumnSimple
{
public:
	explicit ColumnSimple(int index) :
		m_index{ index }
	{}

    auto index() const -> int
    {
        return m_index;
    }

private:
    int m_index;
};

inline auto column(int index) -> ColumnSimple
{
    return ColumnSimple{ index };
}

template<typename CharT>
auto fetch(const BasicRow<CharT>& row, const ColumnSimple column) -> decltype(fetch(row, column.index()))
{
	return fetch(row, column.index());
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_COLUMNSIMPLE_HPP_
