#ifndef RAFALW_CSV_COLUMNMAP_HPP_
#define RAFALW_CSV_COLUMNMAP_HPP_

#include <rafalw/csv/Reader.hpp>
#include <rafalw/csv/ColumnSimple.hpp>
#include <rafalw/csv/ColumnTyped.hpp>
#include <rafalw/csv/Error.hpp>
#include <boost/container/small_vector.hpp>
#include <map>

inline namespace rafalw {
namespace csv {

class ColumnMap
{
public:
    using Data = boost::container::small_vector<Column, 32>;

    class ColumnNotFoundError : public Error
    {
    public:
        ColumnNotFoundError(const boost::string_view& file, const boost::string_view& name) :
            Error{ "column '", name, "' not found in file ", file }
        {}
    };

    ColumnMap(Reader& reader) :
        m_path{ reader.path() }
    {
        auto& line = peek(reader);

        for (auto&& e: line.items())
        {
            m_data.emplace_back(e, m_map.size());
            m_map.emplace(m_data.back().name(), &m_data.back());
        }

        update(reader);
    }

    auto get(const boost::string_view& name) const -> const Column&
    {
        try {
            return *m_map.at(name);
        } catch (const std::out_of_range&) {
            throw ColumnNotFoundError{ m_path, name };
        }
    }

    auto data() const -> const Data&
    {
        return m_data;
    }

private:
    boost::string_view m_path;
    boost::container::small_vector<Column, 32> m_data;
    std::map<boost::string_view, Column*> m_map;
};

inline auto begin(const ColumnMap& cs) -> ColumnMap::Data::const_iterator
{
    return cs.data().begin();
}

inline auto end(const ColumnMap& cs) -> ColumnMap::Data::const_iterator
{
    return cs.data().end();
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_COLUMNMAP_HPP_
