#ifndef XTRAS_UTILS_ITERATORPAIR_HPP_
#define XTRAS_UTILS_ITERATORPAIR_HPP_

namespace xtras {
inline namespace utils {

template<typename _Iterator>
class IteratorPair
{
public:
    using Iterator = _Iterator;

    IteratorPair(Iterator ibegin, Iterator iend) :
        m_begin{ ibegin },
        m_end{ iend }
    {}

    auto begin() const -> Iterator
    {
        return m_begin;
    }

    auto end() const -> Iterator
    {
        return m_end;
    }

private:
    Iterator m_begin;
    Iterator m_end;
};

template<typename Iterator>
auto iterator_pair(Iterator ibegin, Iterator iend) -> IteratorPair<Iterator>
{
    return IteratorPair<Iterator>{ ibegin, iend };
}

} // namespace utils
} // namespace xtras

#endif // XTRAS_UTILS_ITERATORPAIR_HPP_
