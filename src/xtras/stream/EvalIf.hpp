#ifndef XTRAS_STREAM_EVALIF_HPP_
#define XTRAS_STREAM_EVALIF_HPP_

#include <xtras/stream/Eval.hpp>
#include <optional>

namespace xtras {
namespace stream {

template<typename WriteFuncT>
class EvalIf
{
public:
    using WriteFunc = WriteFuncT;
    using Base = Eval<WriteFunc>;

    EvalIf(bool enabled, WriteFunc write)
    {
        if (enabled)
            m_base.emplace(std::move(write));
    }

    template<typename CharT>
    friend auto operator <<(std::basic_ostream<CharT>& stream, const EvalIf& e) -> std::basic_ostream<CharT>&
    {
        if (e.m_base)
            stream << *e.m_base;

        return stream;
    }

private:
    std::optional<Base> m_base;
};

template<typename CondT, typename WriteFuncT>
auto eval_if(const CondT& cond, WriteFuncT write) -> EvalIf<WriteFuncT>
{
    return EvalIf<WriteFuncT>{ static_cast<bool>(cond), std::move(write) };
}

} // namespace stream
} // namespace xtras

#endif // XTRAS_STREAM_EVALIF_HPP_
