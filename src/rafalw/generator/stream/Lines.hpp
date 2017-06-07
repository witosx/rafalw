#ifndef RAFALW_GENERATOR_STREAM_LINES_HPP_
#define RAFALW_GENERATOR_STREAM_LINES_HPP_

#include <rafalw/generator/stream/Items.hpp>
#include <rafalw/streams/Line.hpp>

inline namespace rafalw {
namespace generator {
namespace stream {

template<typename CharT>
using BasicLines = Items<streams::BasicLine<CharT>>;

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
