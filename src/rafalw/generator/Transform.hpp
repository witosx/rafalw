#ifndef RAFALW_GENERATOR_TRANSFORM_HPP_
#define RAFALW_GENERATOR_TRANSFORM_HPP_

#include <rafalw/generator/Modifier.hpp>

inline namespace rafalw {
namespace generator {

template<typename F>
class Transform : public Modifier<Transform<F>>
{
public:
	template<typename F2>
	Transform(F2&& f) :
		m_function{ std::forward<F2>(f) }
	{}

private:
	friend class ModifierAccess;

	using Function = F;

	Function m_function;

	template<typename G>
	auto generatorPeek(const G& gen) const -> decltype(m_function(peek(gen)))
	{
		return m_function(peek(gen));
	}
};

template<typename F>
auto transform(F&& f) -> Transform<std::remove_reference_t<F>>
{
	return Transform<std::remove_reference_t<F>>{ std::forward<F>(f) };
}

template<typename G, typename F, typename = require_instance<G>>
auto operator %(G&& gen, F&& f) -> decltype(std::forward<G>(gen) >> transform(std::forward<F>(f)))
{
	return std::forward<G>(gen) >> transform(std::forward<F>(f));
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_TRANSFORM_HPP_
