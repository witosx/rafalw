#ifndef XTRAS_CSV_COLUMNSIMPLE_HPP_
#define XTRAS_CSV_COLUMNSIMPLE_HPP_

#include <xtras/csv/ColumnError.hpp>
#include <xtras/csv/Row.hpp>

namespace xtras {
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
} // namespace xtras

#endif // XTRAS_CSV_COLUMNSIMPLE_HPP_
