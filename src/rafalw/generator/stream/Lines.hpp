#ifndef RAFALW_GENERATOR_STREAM_LINES_HPP_
#define RAFALW_GENERATOR_STREAM_LINES_HPP_

#include <rafalw/generator/stream/Items.hpp>
#include <rafalw/stream/Line.hpp>

inline namespace rafalw {
namespace generator {
namespace stream {

template<typename CharT>
class BasicLines : private Base
{
public:
    using Char = CharT;
    using Stream = std::basic_istream<Char>;

    BasicLines(Stream& stream, const Char sep) :
        m_items{ stream, sep }
    {}

    explicit BasicLines(Stream& stream) :
        BasicLines{ stream, stream.widen('\n') }
    {}

private:
    friend class generator::BaseAccess;

    Items<rafalw::stream::BasicLine<Char>> m_items;

    auto generatorPeek() const -> decltype(peek(m_items).string())
    {
        return peek(m_items).string();
    }

    auto generatorUpdate() -> void
    {
        update(m_items);
    }

    auto generatorDone() const -> bool
    {
        return done(m_items);
    }
};

using Lines = BasicLines<char>;

template<typename CharT>
auto lines(std::basic_istream<CharT>& stream) -> BasicLines<CharT>
{
    return BasicLines<CharT>{ stream };
}

template<typename CharT>
auto lines(std::basic_istream<CharT>& stream, const CharT sep) -> BasicLines<CharT>
{
    return BasicLines<CharT>{ stream, sep };
}

} // namespace stream
} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_STREAM_LINES_HPP_
