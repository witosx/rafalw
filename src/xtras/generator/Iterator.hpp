#ifndef XTRAS_GENERATOR_ITERATOR_HPP_
#define XTRAS_GENERATOR_ITERATOR_HPP_

namespace xtras {
namespace generator {

class IteratorEnd {};

template<typename GeneratorT>
class Iterator
{
public:
    using Generator = GeneratorT;

    explicit Iterator(Generator& generator) :
        m_generator{ &generator }
    {}

    auto operator ++() -> Iterator&
    {
        update(*m_generator);
        return *this;
    }

    decltype(auto) operator *() const
    {
        return peek(*m_generator);
    }

    auto operator !=(IteratorEnd) const -> bool
    {
        return !done(*m_generator);
    }

private:
    Generator* m_generator;
};

} // namespace generator
} // namespace xtras

#endif /* XTRAS_GENERATOR_ITERATOR_HPP_ */
