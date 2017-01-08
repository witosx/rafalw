#ifndef RAFALW_GENERATOR_ITERATOR_HPP_
#define RAFALW_GENERATOR_ITERATOR_HPP_

#include <rafalw/generator/Generator.hpp>
#include <rafalw/utils/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>

inline namespace rafalw {
namespace generator {

template<typename G>
class Iterator:
		public boost::iterator_facade<
					Iterator<G>,
					GeneratorValue<G>,
					boost::single_pass_traversal_tag,
					GeneratorReference<G>
					>
{
public:
	using Generator = Generator<G>;

	Iterator() = default;

    Iterator(Generator& generator, bool end) :
        m_generator{ &generator },
		m_end{ end || done(generator) }
    {}

private:
    friend class boost::iterator_core_access;

    Generator* m_generator = nullptr;
    bool m_end = false;

    auto increment() -> void
    {
        rafalw_utils_assert(m_generator);
        rafalw_utils_assert(!m_end);

        update(*m_generator);
        m_end = done(*m_generator);
    }

    auto equal(const Iterator& o) const -> bool
    {
        return m_end == o.m_end && m_generator == o.m_generator;
    }

    auto dereference() const -> decltype(peek(*m_generator))
    {
    	rafalw_utils_assert(m_generator);
        return peek(*m_generator);
    }
};

template<typename G>
auto begin(Generator<G>& generator) -> Iterator<G>
{
    return Iterator<G>{ generator, false };
}

template<typename G>
auto end(Generator<G>& generator) -> Iterator<G>
{
    return Iterator<G>{ generator, true };
}

} // namespace generator
} // namespace rafalw

#endif /* RAFALW_GENERATOR_ITERATOR_HPP_ */
