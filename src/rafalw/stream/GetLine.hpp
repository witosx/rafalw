#ifndef RAFALW_STREAM_GETLINE_HPP_
#define RAFALW_STREAM_GETLINE_HPP_

#include <rafalw/traits.hpp>
#include <string>

inline namespace rafalw {
namespace stream {

template<typename CharT>
class BasicGetLine
{
public:
    using Char = CharT;
    using String = std::basic_string<Char>;
    using Stream = std::basic_istream<Char>;

    BasicGetLine(String& string, Char separator) :
        m_string{ string },
        m_separator{ separator }
    {}

    friend auto operator >>(Stream& stream, BasicGetLine& o) -> Stream&
    {
        return std::getline(stream, o.m_string, o.m_separator);
    }

    friend auto operator >>(Stream& stream, BasicGetLine&& o) -> Stream&
    {
        return stream >> o;
    }

private:
    String& m_string;
    Char m_separator;
};

template<typename CharT>
auto getline(std::basic_string<CharT>& string, traits::identity<CharT> separator = '\n') -> BasicGetLine<CharT>
{
    return BasicGetLine<CharT>{ string, separator };
}

using GetLine = BasicGetLine<char>;

} // namespace stream
} // namespace rafalw

#endif // RAFALW_STREAM_GETLINE_HPP_
