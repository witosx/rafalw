#ifndef RAFALW_GENERATOR_BASE_HPP_
#define RAFALW_GENERATOR_BASE_HPP_

#include <rafalw/generator/BaseAccess.hpp>
#include <rafalw/generator/Iterator.hpp>
#include <rafalw/utils/ScopeGuard.hpp>
#include <rafalw/utils/assert.hpp>

inline namespace rafalw {
namespace generator {

struct Base {};

template<typename T>
using is_instance_t = typename std::is_base_of<Base, T>::type;

template<typename T>
constexpr auto is_instance = is_instance_t<T>::value;

template<typename T>
using require_instance = std::enable_if_t<is_instance<T>, std::nullptr_t>;

template<typename G, require_instance<G> = nullptr>
auto done(const G& g) -> bool
{
	return BaseAccess::done(g);
}

template<typename G, require_instance<G> = nullptr>
auto peek(const G& g) -> decltype(BaseAccess::peek(g))
{
    rafalw_utils_assert(!done(g));
	return BaseAccess::peek(g);
}

template<typename G, require_instance<G> = nullptr>
auto update(G& g) -> void
{
    rafalw_utils_assert(!done(g));
	return BaseAccess::update(g);
}

template<typename G, require_instance<G> = nullptr>
auto valid(const G& g) -> bool
{
    return !done(g);
}

template<typename G, require_instance<G> = nullptr>
auto next(G& g) -> std::remove_reference_t<decltype(peek(g))>
{
    const auto deferred_update = utils::scope_guard([&g]{ update(g); });

    return peek(g);
}

template<typename G, require_instance<G> = nullptr>
auto begin(G& g) -> decltype(iterator(g, false))
{
    return iterator(g, false);
}

template<typename G, require_instance<G> = nullptr>
auto end(G& g) -> decltype(iterator(g, true))
{
	return iterator(g, true);
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_BASE_HPP_
