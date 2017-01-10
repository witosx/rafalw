#ifndef RAFALW_GENERATOR_BASEACCESS_HPP_
#define RAFALW_GENERATOR_BASEACCESS_HPP_

inline namespace rafalw {
namespace generator {

class BaseAccess
{
public:
	template<typename T>
	static auto done(const T& g) -> decltype(g.generatorDone())
	{
		return g.generatorDone();
	}

	template<typename T>
	static auto peek(const T& g) -> decltype(g.generatorPeek())
	{
		return g.generatorPeek();
	}

	template<typename T>
	static auto update(T& g) -> decltype(g.generatorUpdate())
	{
		return g.generatorUpdate();
	}
};

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_BASEACCESS_HPP_
