#ifndef RAFALW_CSV_COLUMN_HPP_
#define RAFALW_CSV_COLUMN_HPP_

#include <boost/utility/string_view.hpp>
#include <string>

inline namespace rafalw {
namespace csv {

class Column
{
public:
    Column(const boost::string_view& name, int index) :
        m_name{ name },
        m_index{ index }
    {}

    auto name() const -> const std::string&
    {
        return m_name;
    }

    auto index() const -> int
    {
        return m_index;
    }

private:
    std::string m_name;
    int m_index;
};

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_COLUMN_HPP_
