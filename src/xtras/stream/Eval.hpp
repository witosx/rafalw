#ifndef XTRAS_STREAM_EVAL_HPP_
#define XTRAS_STREAM_EVAL_HPP_

#include <iosfwd>

namespace xtras {
namespace stream {

template<typename WriteFuncT>
class Eval
{
public:
    using WriteFunc = WriteFuncT;

    Eval(WriteFunc write) :
        m_write{ std::move(write) }
    {}

    template<typename CharT>
    friend auto operator <<(std::basic_ostream<CharT>& stream, const Eval& e) -> std::basic_ostream<CharT>&
    {
        e.m_write(stream);
        return stream;
    }

private:
    WriteFunc m_write;
};

template<typename WriteFuncT>
auto eval(WriteFuncT write) -> Eval<WriteFuncT>
{
    return Eval<WriteFuncT>{ std::move(write) };
}

} // namespace stream
} // namespace xtras

#endif // XTRAS_STREAM_EVAL_HPP_
