#ifndef RAFALW_GENERATOR_GENERATORBASE_HPP_
#define RAFALW_GENERATOR_GENERATORBASE_HPP_

#include <rafalw/generator/AccessProxy.hpp>
#include <rafalw/generator/Iterator.hpp>
#include <rafalw/utils/ScopeGuard.hpp>
#include <rafalw/utils/assert.hpp>

inline namespace rafalw {
namespace generator {

class GeneratorBase {};

template<typename T>
class Generator : public GeneratorBase {};

template<typename G>
using is_instance_t = typename std::is_base_of<GeneratorBase, std::remove_reference_t<G>>::type;

template<typename G>
constexpr auto is_instance = is_instance_t<G>::value;

template<typename T>
using require_instance = std::enable_if_t<is_instance<T>>;

template<typename T>
auto upcast(const Generator<T>& g) -> const T&
{
	return static_cast<const T&>(g);
}

template<typename T>
auto upcast(Generator<T>& g) -> T&
{
	return static_cast<T&>(g);
}

template<typename T>
auto done(const Generator<T>& g) -> bool
{
	return AccessProxy::done(upcast(g));
}

template<typename T>
auto valid(const Generator<T>& g) -> bool
{
	return !done(g);
}

template<typename T>
auto peek(const Generator<T>& g) -> decltype(AccessProxy::peek(upcast(g)))
{
    rafalw_utils_assert(!done(g));
	return AccessProxy::peek(upcast(g));
}

template<typename T>
auto update(Generator<T>& g) -> void
{
    rafalw_utils_assert(!done(g));
	return AccessProxy::update(upcast(g));
}

template<typename T>
auto next(Generator<T>& g) -> std::remove_reference_t<decltype(peek(g))>
{
    const auto deferred_update = utils::scope_guard([&g]{
        update(g);
    });

    return peek(g);
}

template<typename T>
auto begin(Generator<T>& g) -> decltype(iterator(upcast(g), false))
{
    return iterator(upcast(g), false);
}

template<typename T>
auto end(Generator<T>& g) -> decltype(iterator(upcast(g), true))
{
	return iterator(upcast(g), true);
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_GENERATORBASE_HPP_
