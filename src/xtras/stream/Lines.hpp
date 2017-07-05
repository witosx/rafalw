#ifndef XTRAS_STREAM_LINES_HPP_
#define XTRAS_STREAM_LINES_HPP_

#include <xtras/stream/Items.hpp>
#include <xtras/stream/Line.hpp>
#include <xtras/generator/Base.hpp>

namespace xtras {
namespace stream {

template<typename CharT>
class BasicLines : private generator::Base
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

    Items<BasicLine<Char>> m_items;

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
} // namespace xtras

#endif // XTRAS_STREAM_LINES_HPP_
