#ifndef RAFALW_GENERATOR_SEQUENCE_HPP_
#define RAFALW_GENERATOR_SEQUENCE_HPP_

#include <rafalw/generator/Base.hpp>
#include <limits>

inline namespace rafalw {
namespace generator {

template<typename IndexT, typename StepT = IndexT>
class Sequence : private Base
{
public:
    using Index = IndexT;
    using Step = StepT;

    Sequence(const Index begin, const Index end, const Step step) :
        m_begin{ begin },
        m_end{ end },
        m_step{ step }
    {}

private:
    friend class BaseAccess;

    Index m_begin;
    Index m_end;
    Step m_step;

    Index m_current = m_begin;

    auto generatorDone() const -> bool
    {
        return !(m_current < m_end);
    }

    auto generatorPeek() const -> const Index&
    {
        return m_current;
    }

    auto generatorUpdate() -> void
    {
        m_current += m_step;
    }

    auto generatorReset()
    {
        m_current = m_begin;
        return RESET_OK;
    }
};

template<typename Index, typename Step>
inline auto sequence(Index begin, Index end, Step step) -> Sequence<Index, Step>
{
    return Sequence<Index, Step>{ begin, end, step };
}

template<typename Index>
inline auto sequence(Index begin, Index end) -> decltype(sequence(begin, end, decltype(std::declval<Index>() - std::declval<Index>()){ 1 }))
{
    return sequence(begin, end, decltype(std::declval<Index>() - std::declval<Index>()){ 1 });
}

template<typename Index>
inline auto sequence(Index end) -> decltype(sequence(Index{ 0 }, end))
{
    return sequence(Index{ 0 }, end);
}

template<typename Index = std::size_t>
inline auto sequence() -> decltype(sequence(std::numeric_limits<Index>::max()))
{
    return sequence(std::numeric_limits<Index>::max());
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_SEQUENCE_HPP_
