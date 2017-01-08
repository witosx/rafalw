#ifndef RAFALW_GENERATOR_GENERATORACCESS_HPP_
#define RAFALW_GENERATOR_GENERATORACCESS_HPP_

inline namespace rafalw {
namespace generator {

class GeneratorAccess
{
public:
	template<typename T>
	friend class Generator;

	template<typename T>
	static decltype(auto) done(const T& g)
	{
		return g.generatorDone();
	}

	template<typename T>
	static decltype(auto) peek(const T& g)
	{
		return g.generatorPeek();
	}

	template<typename T>
	static decltype(auto) update(T& g)
	{
		return g.generatorUpdate();
	}
};

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_GENERATORACCESS_HPP_
