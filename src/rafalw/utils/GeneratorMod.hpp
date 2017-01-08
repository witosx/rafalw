#ifndef RAFALW_UTILS_GENERATORMOD_HPP_
#define RAFALW_UTILS_GENERATORMOD_HPP_

#include <rafalw/utils/Generator.hpp>

inline namespace rafalw {
namespace utils {

class GeneratorModAccess
{
public:
	template<typename M, typename G>
	static auto done(const M& mod, const G& gen) -> decltype(mod.generatorDone(gen))
	{
		return mod.generatorDone(gen);
	}

	template<typename M, typename G>
	static auto peek(const M& mod, const G& gen) -> decltype(mod.generatorPeek(gen))
	{
		return mod.generatorPeek(gen);
	}

	template<typename M, typename G>
	static auto update(M& mod, G& gen) -> decltype(mod.generatorUpdate(gen))
	{
		return mod.generatorUpdate(gen);
	}
};

template<typename DerivedT>
class GeneratorMod
{
private:
	using Derived = DerivedT;

	auto derived() -> Derived&
	{
		return static_cast<Derived&>(*this);
	}

	auto derived() const -> const Derived&
	{
		return static_cast<const Derived&>(*this);
	}

public:
	template<typename G>
	auto done(const G& gen) const -> decltype(GeneratorModAccess::done(derived(), gen))
	{
		return GeneratorModAccess::done(derived(), gen);
	}

	template<typename G>
	auto peek(const G& gen) const -> decltype(GeneratorModAccess::peek(derived(), gen))
	{
		return GeneratorModAccess::peek(derived(), gen);
	}

	template<typename G>
	auto update(G& gen) -> decltype(GeneratorModAccess::update(derived(), gen))
	{
		return GeneratorModAccess::update(derived(), gen);
	}

protected:
	template<typename G>
	auto generatorDone(const G& gen) const -> decltype(gen.done())
	{
		return gen.done();
	}

	template<typename G>
	auto generatorPeek(const G& gen) const -> decltype(gen.peek())
	{
		return gen.peek();
	}

	template<typename G>
	auto generatorUpdate(G& gen) -> decltype(gen.update())
	{
		return gen.update();
	}
};

template<typename G, typename M>
class GeneratorModified : public Generator<GeneratorModified<G, M>>
{
public:
	template<typename G2, typename M2>
	GeneratorModified(G2&& g, M2&& m) :
		m_generator{ std::forward<G2>(g) },
		m_modifier{ std::forward<M2>(m) }
	{}

private:
	friend class GeneratorAccess;

	using BaseGenerator = G;
	using BaseModifier = M;

	BaseGenerator m_generator;
	BaseModifier m_modifier;

    auto generatorDone() const -> decltype(m_modifier.done(m_generator))
    {
    	return m_modifier.done(m_generator);
    }

    auto generatorPeek() const -> decltype(m_modifier.peek(m_generator))
    {
    	return m_modifier.peek(m_generator);
    }

    auto generatorUpdate() -> decltype(m_modifier.update(m_generator))
    {
    	return m_modifier.update(m_generator);
    }
};

template<typename D, typename M>
auto operator >>(Generator<D>& gen, GeneratorMod<M> mod) -> GeneratorModified<Generator<D>&, GeneratorMod<M>>
{
	return GeneratorModified<Generator<D>&, GeneratorMod<M>>{ gen, mod };
}

template<typename D, typename M>
auto operator >>(Generator<D>&& gen, GeneratorMod<M> mod) -> GeneratorModified<Generator<D>&&, GeneratorMod<M>>
{
	return GeneratorModified<Generator<D>&&, GeneratorMod<M>>{ std::move(gen), mod };
}

template<typename F>
class GeneratorTransform : public GeneratorMod<GeneratorTransform<F>>
{
public:
	template<typename F2>
	GeneratorTransform(F2&& f) :
		m_transform{ std::forward<F2>(f) }
	{}

private:
	friend class GeneratorModAccess;

	using Transform = F;

	Transform m_transform;

	template<typename G>
	auto generatorPeek(const G& gen) const -> decltype(m_transform(gen.peek()))
	{
		return m_transform(gen.peek());
	}
};

template<typename F>
auto generator_transform(F&& f) -> GeneratorTransform<std::remove_reference_t<F>>
{
	return GeneratorTransform<std::remove_reference_t<F>>{ std::forward<F>(f) };
}

template<typename D, typename F>
auto operator %(Generator<D>& gen, F&& f) -> decltype(gen >> generator_transform(std::forward<F>(f)))
{
	return gen >> generator_transform(std::forward<F>(f));
}

template<typename D, typename F>
auto operator %(Generator<D>&& gen, F&& f) -> decltype(std::move(gen) >> generator_transform(std::forward<F>(f)))
{
	return std::move(gen) >> generator_transform(std::forward<F>(f));
}

template<typename F>
class GeneratorFilter : public GeneratorMod<GeneratorFilter<F>>
{
public:
	template<typename F2>
	GeneratorFilter(F2&& f) :
		m_function{ std::forward<F2>(f) }
	{}

private:
	friend class GeneratorModAccess;

	using Function = F;

	Function m_function;

	template<typename G>
	auto generatorUpdate(G& gen) const -> void
	{
		while (true)
		{
			gen.update();

			if (!gen || m_function(gen.peek()))
				break;
		}

	}
};

template<typename F>
auto generator_filter(F&& f) -> GeneratorFilter<std::remove_reference_t<F>>
{
	return GeneratorFilter<std::remove_reference_t<F>>{ std::forward<F>(f) };
}

template<typename D, typename F>
auto operator |(Generator<D>& gen, F&& f) -> decltype(gen >> generator_filter(std::forward<F>(f)))
{
	return gen >> generator_filter(std::forward<F>(f));
}

template<typename D, typename F>
auto operator |(Generator<D>&& gen, F&& f) -> decltype(std::move(gen) >> generator_filter(std::forward<F>(f)))
{
	return std::move(gen) >> generator_filter(std::forward<F>(f));
}

} // namespace utils
} // namespace rafalw

#endif /* RAFALW_UTILS_GENERATORMOD_HPP_ */
