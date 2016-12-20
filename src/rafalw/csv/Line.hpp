#ifndef RAFALW_CSV_LINE_HPP_
#define RAFALW_CSV_LINE_HPP_

#include <rafalw/csv/common.hpp>
#include <rafalw/utils/Error.hpp>
#include <rafalw/utils/demangle.hpp>
#include <string>
#include <sstream>
#include <boost/tokenizer.hpp>

inline namespace rafalw {
namespace csv {

class Line
{
public:
    using Char = char;
    using String = std::basic_string<Char>;

    class Error : public utils::Error
    {
    private:
        int m_field;

    public:
        template<typename... Args>
        Error(int field, Args&&... args) :
            utils::Error{ "CSV Line Error [field=", field, "] - ", args... },
            m_field{ field }
        {}

        auto field() const -> int
        {
            return m_field;
        }
    };

    class NoDataError : public Error
    {
    public:
        NoDataError(int field) :
            Error{ field, "no more data" }
        {}
    };

    class ParseError : public Error
    {
    private:
        std::string m_value;
        std::string m_type;

    public:
        ParseError(int field, const std::string& val, const std::string& type) :
            Error{ field, "can't parse \"", val, "\" as ", type },
            m_value{ val },
            m_type{ type }
        {}

        auto value() const -> const std::string&
        {
            return m_value;
        }

        auto type() const -> const std::string&
        {
            return m_type;
        }
    };

    Line(const String& line, String delimiters, Empty ep, const std::string& filename, int lineno) :
        m_filename{ filename },
        m_number{ lineno },
        m_line{ line }
    {
        auto idx = std::string::size_type{ 0 };

        while (idx != std::string::npos)
        {
            const auto idx_sep = m_line.find_first_of(delimiters, idx);

            if (idx_sep == std::string::npos)
                break;

            m_items.push_back({ m_line.data() + idx, idx_sep - idx });

            if (ep == Empty::KEEP)
                idx = idx_sep + 1;
            else
                idx = m_line.find_first_not_of(delimiters, idx_sep);
        }

        if (idx != std::string::npos)
            m_items.push_back({ m_line.data() + idx, m_line.size() - idx });
    }

    auto string() const -> const std::string&
    {
        return m_line;
    }

    template<typename T>
    auto get(int idx) const -> T
    {
        auto& item = [this,idx]() -> const Item& {
            try {
                return m_items.at(idx);
            } catch (const std::out_of_range&) {
                throw NoDataError{ idx };
            }
        }();

        try {
            return streams::convert<T>(item.begin, item.size);
        } catch (const std::exception& e) {
            throw ParseError{ idx, std::string(item.begin, item.size), utils::demangle<T>() };
        }
    }

private:
    struct Item
    {
        const char* begin;
        std::size_t size;
    };

    const std::string& m_filename;
    int m_number;

    String m_line;

    std::vector<Item> m_items;
};

} // namespace csv
} // inline namespace rafalw

#endif // RAFALW_CSV_LINE_HPP_
