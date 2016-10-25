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

template<typename _Char>
class LineGeneric
{
public:
    using Char = _Char;
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

private:
    using TokenizerSeparator = boost::char_separator<Char>;
    using Tokenizer = boost::tokenizer<TokenizerSeparator>;

    String m_delimiters;
    Tokenizer m_tokenizer;
    typename Tokenizer::iterator m_iterator;
    int m_field = 0;

    std::istringstream m_stream;

public:
    LineGeneric(const String& line_str, String delimiters, Empty ep) :
        m_delimiters{ delimiters },
        m_tokenizer{ line_str, TokenizerSeparator{ m_delimiters.c_str(), "", ep == Empty::KEEP ? boost::keep_empty_tokens : boost::drop_empty_tokens } },
        m_iterator{ m_tokenizer.begin() }
    {}

    template<typename T>
    auto operator >>(T& obj) -> LineGeneric&
    {
        if (m_iterator == m_tokenizer.end())
            throw NoDataError{ m_field };

        m_field++;

        m_stream.clear();
        m_stream.str(*m_iterator);

        try {
            m_stream >> obj;
        } catch (const std::exception& e) {
            throw ParseError{ m_field, *m_iterator, utils::demangle<T>() };
        }

        if (!m_stream)
            throw ParseError{ m_field, *m_iterator, utils::demangle<T>() };

        ++m_iterator;
        return *this;
    }

    template<typename T>
    auto get() -> T
    {
        auto val = T{};
        *this >> val;
        return val;
    }

    auto skip() -> void
    {
        if (m_iterator == m_tokenizer.end())
            throw NoDataError{ m_field };

        ++m_field;
        ++m_iterator;
    }

    operator bool() const
    {
        return m_iterator < m_tokenizer.end();
    }

    auto operator !() const -> bool
    {
        return !static_cast<bool>(*this);
    }
};

using Line = LineGeneric<char>;

} // namespace csv
} // inline namespace rafalw

#endif // RAFALW_CSV_LINE_HPP_
