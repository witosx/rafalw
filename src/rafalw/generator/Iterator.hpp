#ifndef RAFALW_GENERATOR_ITERATOR_HPP_
#define RAFALW_GENERATOR_ITERATOR_HPP_

#include <rafalw/utils/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>

inline namespace rafalw {
namespace generator {

namespace detail {

	template<typename G>
	using iterator_reference = decltype(peek(std::declval<G>()));

	template<typename G>
	using iterator_value = std::remove_reference_t<iterator_reference<G>>;

	using iterator_tag = boost::single_pass_traversal_tag;

} // namespace detail

template<typename G>
class Iterator:
		public boost::iterator_facade<
					Iterator<G>,
					detail::iterator_value<G>,
					detail::iterator_tag,
					detail::iterator_reference<G>
					>
{
public:
	using Generator = G;

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
auto iterator(G& g, bool end) -> Iterator<G>
{
	return Iterator<G>{ g, end };
}

} // namespace generator
} // namespace rafalw

#endif /* RAFALW_GENERATOR_ITERATOR_HPP_ */
