#ifndef RAFALW_STREAMS_LINES_HPP_
#define RAFALW_STREAMS_LINES_HPP_

#include <rafalw/generator/Generator.hpp>
#include <rafalw/utils/Error.hpp>
#include <string>
#include <istream>

inline namespace rafalw {
namespace streams {

template<typename CharT>
class Lines : public generator::Generator<Lines<CharT>>
{
public:
	using Char = CharT;
	using Stream = std::basic_istream<Char>;
	using String = std::basic_string<Char>;

	class Error : public utils::Error
	{
	public:
		using utils::Error::Error;
	};

    Lines(Stream& stream, Char sep = '\n') :
        m_stream{ stream },
		m_separator{ sep }
    {
    	generatorUpdate();
    }

private:
    friend class generator::AccessProxy;

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

        if (!m_stream && !m_stream.eof())
        	throw Error{ "Lines: read error "};
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

} // namespace streams
} // namespace rafalw

#endif // RAFALW_STREAMS_LINES_HPP_
