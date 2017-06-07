#ifndef RAFALW_STREAMS_LINE_HPP_
#define RAFALW_STREAMS_LINE_HPP_

#include <string>

inline namespace rafalw {
namespace streams {

template<typename CharT>
class BasicLine
{
public:
    using Char = CharT;
    using String = std::basic_string<Char>;
    using Stream = std::basic_istream<Char>;

    explicit BasicLine(Char separator = '\n') :
        m_separator{ separator }
    {}

    auto string() const -> const String&
    {
        return m_string;
    }

    operator const String& () const
    {
        return m_string;
    }

    friend auto operator >>(Stream& stream, BasicLine& line) -> Stream&
    {
        return std::getline(stream, line.m_string, line.m_separator);
    }

private:
    String m_string;
    Char m_separator;
};

using Line = BasicLine<char>;

} // namespace streams
} // namespace rafalw

#endif // RAFALW_STREAMS_LINE_HPP_

