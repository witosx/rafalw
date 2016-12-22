#ifndef RAFALW_CSV_LINE_HPP_
#define RAFALW_CSV_LINE_HPP_

#include <rafalw/csv/common.hpp>
#include <rafalw/utils/Error.hpp>
#include <rafalw/utils/demangle.hpp>
#include <rafalw/strings/Tokens.hpp>
#include <rafalw/strings/parse.hpp>
#include <string>
#include <sstream>
#include <vector>

inline namespace rafalw {
namespace csv {

class Line
{
public:
    using Char = char;
    using String = std::basic_string<Char>;
    using StringView = boost::basic_string_view<Char>;

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

    Line(const String& line, const StringView& delimiters, Empty ep, const std::string& filename, int lineno) :
        m_line{ line },
        m_filename{ filename },
        m_number{ lineno }
    {
        for (auto&& token: strings::Tokens{ m_line, delimiters })
        {
            if (ep == Empty::IGNORE && token.empty())
                continue;

            m_items.push_back(token);
        }
    }

    auto string() const -> const std::string&
    {
        return m_line;
    }

    template<typename T>
    auto get(int idx) const -> T
    {
        return strings::parse<T>(get(idx));
    }

    auto get(int idx) const -> const StringView&
    {
        try {
            return m_items.at(idx);
        } catch (const std::out_of_range&) {
            throw NoDataError{ idx };
        }
    }

private:
    String m_line;

    const std::string& m_filename;
    int m_number;

    std::vector<StringView> m_items;
};

} // namespace csv
} // inline namespace rafalw

#endif // RAFALW_CSV_LINE_HPP_
