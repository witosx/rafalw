#ifndef RAFALW_CSV_LINE_HPP_
#define RAFALW_CSV_LINE_HPP_

#include <rafalw/csv/Error.hpp>
#include <rafalw/generator/string/Tokens.hpp>
#include <rafalw/strings/parse.hpp>
#include <rafalw/utils/demangle.hpp>
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
    using StringView = std::experimental::basic_string_view<Char>;

    enum class EmptyPolicy
    {
        KEEP,
        IGNORE
    };

    class Error : public utils::Error
    {
    public:
        template<typename... Args>
        Error(const std::string& fname, int lineno, int field, Args&&... args) :
            utils::Error{ "csv error ", fname, ":", lineno, ":", field, " ", args... }
        {}
    };

    class NoDataError : public Error
    {
    public:
        NoDataError(const std::string& fname, int lineno, int field) :
            Error{ fname, lineno, field, "no more data" }
        {}
    };

    Line(const String& line, const StringView& delimiters, EmptyPolicy ep, const std::string& filename, int lineno) :
        m_line{ line },
        m_filename{ filename },
        m_number{ lineno }
    {
        for (auto&& token: generator::string::tokens(m_line, delimiters))
        {
            if (ep == EmptyPolicy::IGNORE && token.empty())
                continue;

            m_items.push_back(token);
        }
    }

    auto number() const -> int
    {
        return m_number;
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
            throw NoDataError{ m_filename, m_number, idx };
        }
    }

    auto items() const -> const std::vector<StringView>&
    {
        return m_items;
    }

private:
    String m_line;

    const std::string& m_filename;
    int m_number;

    std::vector<StringView> m_items;
};

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_LINE_HPP_
