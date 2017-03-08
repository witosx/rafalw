#ifndef RAFALW_STREAMS_EVAL_HPP_
#define RAFALW_STREAMS_EVAL_HPP_

#include <iosfwd>

inline namespace rafalw {
namespace streams {

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

} // namespace streams
} // namespace rafalw

#endif // RAFALW_STREAMS_EVAL_HPP_
