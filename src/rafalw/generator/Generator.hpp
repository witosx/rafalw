#ifndef RAFALW_GENERATOR_GENERATOR_HPP_
#define RAFALW_GENERATOR_GENERATOR_HPP_

#include <rafalw/generator/GeneratorAccess.hpp>
#include <rafalw/utils/ScopeGuard.hpp>
#include <rafalw/utils/assert.hpp>

inline namespace rafalw {
namespace generator {

template<typename DerivedT>
class Generator
{
public:
    operator bool() const
    {
        return !done(*this);
    }
};

template<typename G>
auto done(const Generator<G>& g) -> bool
{
	return GeneratorAccess::done(static_cast<const G&>(g));
}

template<typename G>
auto peek(const Generator<G>& g) -> decltype(GeneratorAccess::peek(static_cast<const G&>(g)))
{
    rafalw_utils_assert(!done(g));
	return GeneratorAccess::peek(static_cast<const G&>(g));
}

template<typename G>
auto update(Generator<G>& g) -> void
{
    rafalw_utils_assert(!done(g));
	return GeneratorAccess::update(static_cast<G&>(g));
}

template<typename G>
auto next(Generator<G>& g) -> std::remove_reference_t<decltype(peek(g))>
{
    auto deferred_update = utils::scope_guard([&g]{
        update(g);
    });

    return peek(g);
}

template<typename G>
auto operator !(const Generator<G>& g) -> bool
{
    return done(g);
}

namespace detail {

    template<typename T>
    struct GeneratorTraits;

    template<typename G>
    struct GeneratorTraits<Generator<G>>
    {
        using Reference = decltype(peek(std::declval<Generator<G>>()));
        using Value = std::remove_reference_t<Reference>;
    };

} // namespace detail


template<typename T>
using GeneratorReference = typename detail::GeneratorTraits<Generator<T>>::Reference;

template<typename T>
using GeneratorValue = typename detail::GeneratorTraits<Generator<T>>::Value;

} // namespace generator
} // namespace rafalw

#include <rafalw/generator/Iterator.hpp>

#endif // RAFALW_GENERATOR_GENERATOR_HPP_
