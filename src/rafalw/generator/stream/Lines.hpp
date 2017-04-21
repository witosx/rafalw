#ifndef RAFALW_GENERATOR_STREAM_LINES_HPP_
#define RAFALW_GENERATOR_STREAM_LINES_HPP_

#include <rafalw/generator/Base.hpp>
#include <string>
#include <istream>

inline namespace rafalw {
namespace generator {
namespace stream {

template<typename CharT = char>
class Lines : private Base
{
public:
    using Char = CharT;
    using Stream = std::basic_istream<Char>;
    using String = std::basic_string<Char>;

    Lines(Stream& stream, Char sep = '\n') :
        m_stream{ stream },
        m_separator{ sep }
    {
        generatorUpdate();
    }

private:
    friend class generator::BaseAccess;

    Stream& m_stream;
    String m_line;
    Char m_separator;

    auto generatorPeek() const -> const String&
    {
        return m_line;
    }

    auto generatorUpdate() -> void
    {
        getline(m_stream, m_line, m_separator);
    }

    auto generatorDone() const -> bool
    {
        return m_stream.fail() || m_stream.bad();
    }
};

template<typename Char>
auto lines(std::basic_istream<Char>& stream, Char sep = '\n') -> Lines<Char>
{
    return Lines<Char>{ stream, sep };
}

} // namespace stream
} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_STREAM_LINES_HPP_
