#ifndef RAFALW_STREAMS_LINES_HPP_
#define RAFALW_STREAMS_LINES_HPP_

#include <rafalw/utils/Generator.hpp>
#include <string>
#include <istream>

inline namespace rafalw {
namespace streams {

template<typename CharT>
class Lines : public utils::Generator<Lines<CharT>, std::basic_string<CharT>>
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
    friend class utils::GeneratorAccess;

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
        return (m_stream.rdstate() & (std::ios_base::failbit | std::ios_base::badbit)) != 0;
    }
};

template<typename Char>
auto lines(std::basic_istream<Char>& stream, Char sep = '\n') -> Lines<Char>
{
	return Lines<Char>{ stream, sep };
}

} // namespace streams
} // namespace rafalw

#endif // RAFALW_STREAMS_LINES_HPP_
