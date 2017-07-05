#ifndef XTRAS_STREAM_LOAD_HPP_
#define XTRAS_STREAM_LOAD_HPP_

#include <iosfwd>

namespace xtras {
namespace stream {

template<typename ParseFunctionT>
class Load
{
public:
    using ParseFunction = ParseFunctionT;

    Load(ParseFunction parse) :
        m_parse{ std::move(parse) }
    {}

    template<typename CharT>
    friend auto operator >>(std::basic_istream<CharT>& stream, Load& e) -> std::basic_istream<CharT>&
    {
        e.m_parse(stream);
        return stream;
    }

private:
    ParseFunction m_parse;
};

template<typename ParseFunctionT>
auto load(ParseFunctionT parse) -> Load<ParseFunctionT>
{
    return Load<ParseFunctionT>{ std::move(parse) };
}

} // namespace stream
} // namespace xtras

#endif // XTRAS_STREAM_LOAD_HPP_
