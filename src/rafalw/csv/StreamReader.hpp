#ifndef RAFALW_CSV_STREAMREADER_HPP_
#define RAFALW_CSV_STREAMREADER_HPP_

#include <rafalw/csv/Context.hpp>
#include <rafalw/csv/Row.hpp>
#include <rafalw/csv/Error.hpp>
#include <rafalw/generator/Base.hpp>
#include <rafalw/stream/Lines.hpp>
#include <string>
#include <string_view>

inline namespace rafalw {
namespace csv {

template<typename CharT>
class BasicStreamReader : private generator::Base
{
public:
    using Char = CharT;
    using Stream = std::basic_istream<Char>;
    using String = std::basic_string<Char>;
    using StringView = std::basic_string_view<Char>;

    using Row = BasicRow<CharT>;

    BasicStreamReader(Stream& stream, std::string_view source_name, StringView row_delimiters, empty_fields row_keep_empty = empty_fields::keep) :
        m_context{ source_name },
        m_lines{ stream },
        m_row{ m_context, row_delimiters, row_keep_empty }
    {
        processCurrentLine();
    }

    BasicStreamReader(Stream& stream, StringView row_delimiters, empty_fields row_keep_empty = empty_fields::keep) :
        BasicStreamReader{ stream, "<unnamed-stream>", row_delimiters, row_keep_empty }
    {}

    auto context() const -> const Context&
    {
        return m_context;
    }

private:
    friend class generator::BaseAccess;

    Context m_context;
    stream::BasicLines<Char> m_lines;
    Row m_row;

    auto generatorDone() const -> bool
    {
        return done(m_lines);
    }

    auto generatorPeek() const -> const Row&
    {
        return m_row;
    }

    auto generatorUpdate() -> void
    {
        m_context.line++;
        update(m_lines);
        processCurrentLine();
    }

    auto readLine() const -> const String&
    {
        try {
            return peek(m_lines);
        }
        catch (const stream::Error&) {
            throw Error{ m_context, "read failed" };
        }
    }

    auto processCurrentLine() -> void
    {
        if (done(m_lines))
            return;

        m_row.parse(readLine());
    }
};

using StreamReader = BasicStreamReader<char>;

template<typename CharT>
auto reader(std::basic_istream<CharT>& stream, typename BasicStreamReader<CharT>::StringView row_delimiters, empty_fields row_keep_empty = empty_fields::keep) -> BasicStreamReader<CharT>
{
    return BasicStreamReader<CharT>{ stream, row_delimiters, row_keep_empty };
}

template<typename CharT>
auto reader(std::basic_istream<CharT>& stream, std::string_view source_name, typename BasicStreamReader<CharT>::StringView row_delimiters, empty_fields row_keep_empty = empty_fields::keep) -> BasicStreamReader<CharT>
{
    return BasicStreamReader<CharT>{ stream, source_name, row_delimiters, row_keep_empty };
}

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_STREAMREADER_HPP_
