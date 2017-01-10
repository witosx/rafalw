#ifndef RAFALW_CSV_READER_HPP_
#define RAFALW_CSV_READER_HPP_

#include <rafalw/csv/Line.hpp>
#include <rafalw/io/TextInputFile.hpp>
#include <rafalw/generator/Base.hpp>
#include <rafalw/utils/Error.hpp>
#include <rafalw/utils/assert.hpp>
#include <rafalw/utils/ScopeGuard.hpp>
#include <boost/optional.hpp>
#include <string>

inline namespace rafalw {
namespace csv {

template<typename CharT>
class ReaderBasic : private generator::Base
{
public:
    using Char = CharT;
    using String = std::basic_string<Char>;

    ReaderBasic(const std::string& path, String delimiters, Empty ep = Empty::KEEP) :
        m_file{ path },
        m_delimiters{ delimiters },
        m_emptyPolicy{ ep }
    {
        generatorUpdate();
    }

    auto path() const -> const std::string&
    {
        return m_file.path();
    }

private:
    friend class generator::BaseAccess;

    int m_lineno = 1;
    io::TextInputFile m_file;
    io::TextInputFile::Lines m_lines{ m_file.lines() };

    String m_delimiters;
    Empty m_emptyPolicy;
    boost::optional<Line> m_line;

    auto generatorDone() const -> bool
    {
        return !m_line;
    }

    auto generatorPeek() const -> const Line&
    {
        return *m_line;
    }

    auto generatorUpdate() -> void
    {
        if (done(m_lines))
        {
            m_line.reset();
            return;
        }

        const auto sg = utils::scope_guard([this]{
            update(m_lines);
            m_lineno++;
        });

        m_line.emplace(peek(m_lines), m_delimiters, m_emptyPolicy, m_file.path(), m_lineno);
    }
};

using Reader = ReaderBasic<char>;

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_READER_HPP_
