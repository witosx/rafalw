#ifndef RAFALW_CSV_READERBASE_HPP_
#define RAFALW_CSV_READERBASE_HPP_

#include <rafalw/csv/Line.hpp>
#include <rafalw/io/TextInputFile.hpp>
#include <rafalw/utils/Generator.hpp>
#include <rafalw/utils/Error.hpp>
#include <boost/optional.hpp>
#include <string>
#include <cassert>

inline namespace rafalw {
namespace csv {

template<typename Impl, typename T, bool skip_init = false>
class ReaderBase : public utils::Generator<ReaderBase<Impl, T, skip_init>, T>
{
public:
    using Entry = T;
    using Char = char;
    using String = std::basic_string<Char>;
    using Line = LineGeneric<Char>;

    class Error : public utils::Error
    {
    public:
        template<typename... Args>
        Error(const std::string& path, Args&&... args) :
            utils::Error{ "CSV Reader Error [", path, args... }
        {}
    };

    class OpenError : public Error
    {
    public:
        OpenError(const std::string& path) :
            Error{ path, "]: can't open file" }
        {}
    };

    class LineError : public Error
    {
    public:
        template<typename... Args>
        LineError(const std::string& path, int line, Args&&... args) :
            Error{ path, ":", line, "]: ", args... }
        {}
    };

private:
    int m_lineno = 0;
    io::TextInputFile m_file;

    String m_delimiters;
    Empty m_emptyPolicy;
    boost::optional<Entry> m_entry;
    boost::optional<LineError> m_error;

public:
    ReaderBase(const std::string& path, String delimiters, Empty ep = Empty::KEEP) try :
        m_file{ path },
        m_delimiters{ delimiters },
        m_emptyPolicy{ ep }
    {
        if (!skip_init)
            update();
    }
    catch (const io::TextInputFile::OpenError& e)
    {
        throw OpenError{ path };
    }

    auto done() const -> bool
    {
        return !m_entry && !m_error;
    }

    auto peek() const -> const Entry&
    {
        assert(!done());

        if (m_error)
            throw *m_error;

        return *m_entry;
    }

    auto update() -> void
    {
        m_entry = boost::optional<Entry>{};
        m_error = boost::optional<LineError>{};

        for (auto& line: m_file.lines())
        {
            m_lineno++;

            if (!line.empty())
            {
                try {
                    m_entry = static_cast<Impl*>(this)->processLine(Line{ line, m_separator, m_emptyPolicy });
                } catch (const Line::NoDataError& e) {
                    m_error = LineError{ m_file.path(), m_lineno, "field ", e.field(), " no more data",  };
                } catch (const Line::ParseError& e) {
                    m_error = LineError{ m_file.path(), m_lineno, "field ", e.field(), "can't parse \"", e.value(), "\" as ", e.type() };
                } catch (const std::exception& e) {
                    m_error = LineError{ m_file.path(), m_lineno, "line processing error: ", e.what() };
                }

                break;
            }
        }
    }
};

} // namespace csv
} // namespace rafalw

#endif // RAFALW_CSV_READERBASE_HPP_
