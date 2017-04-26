#ifndef RAFALW_GENERATOR_BASE_HPP_
#define RAFALW_GENERATOR_BASE_HPP_

#include <rafalw/generator/Iterator.hpp>
#include <rafalw/utils/ScopeGuard.hpp>
#include <rafalw/utils/assert.hpp>

inline namespace rafalw {
namespace generator {

struct ConstructTag {};

template<bool OK>
struct ResetTag : public std::bool_constant<OK> {};

template<bool OK>
static constexpr auto RESET_TAG = ResetTag<OK>{};

struct Base
{
protected:
    auto generatorReset()
    {
        return RESET_TAG<false>;
    }
};

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

    template<typename T>
    static auto reset(T& g) -> decltype(g.generatorReset())
    {
        return g.generatorReset();
    }
};

template<typename T>
using is_instance_t = typename std::is_base_of<Base, T>::type;

template<typename T>
constexpr auto is_generator = is_instance_t<std::decay_t<T>>::value;

template<typename T>
using require_instance = std::enable_if_t<is_generator<T>, std::nullptr_t>;

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
constexpr auto try_reset(G& g) -> decltype(BaseAccess::reset(g))
{
    return BaseAccess::reset(g);
}

template<typename G, require_instance<G> = nullptr>
using reset_tag = decltype(try_reset(std::declval<G&>()));

template<typename G>
constexpr auto reset_ok = reset_tag<G>::value;

template<typename G, require_instance<G> = nullptr, std::enable_if_t<reset_ok<G>>* = nullptr>
auto reset(G& g) -> void
{
    try_reset(g);
}

template<typename G, require_instance<G> = nullptr>
auto valid(const G& g) -> bool
{
    return !done(g);
}

template<typename G, require_instance<G> = nullptr>
auto next(G& g) -> std::remove_reference_t<decltype(peek(g))>
{
    const auto sg = utils::scope_guard([&g]{ update(g); });
    return peek(g);
}

template<typename G, require_instance<G> = nullptr>
auto begin(G& g) -> Iterator<G>
{
    return Iterator<G>{ g };
}

template<typename G, require_instance<G> = nullptr>
auto end(G&) -> IteratorEnd
{
	return IteratorEnd{};
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_BASE_HPP_
