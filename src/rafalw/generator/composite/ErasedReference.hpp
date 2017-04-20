#ifndef RAFALW_GENERATOR_ERASEDREFERENCE_HPP_
#define RAFALW_GENERATOR_ERASEDREFERENCE_HPP_

#include <rafalw/generator/Base.hpp>
#include <rafalw/utils/FunctionPtr.hpp>
#include <rafalw/utils/assert.hpp>
#include <boost/optional.hpp>

inline namespace rafalw {
namespace generator {

namespace detail {

template<typename ElementT>
struct ErasedReferenceElement
{
    ErasedReferenceElement() = default;
    ErasedReferenceElement(ElementT&& e) :
        m_data{ std::move(e) }
    {}

    auto data() const -> const ElementT*
    {
        return m_data.ptr();
    }

    boost::optional<ElementT> m_data;
};

template<typename ElementT>
struct ErasedReferenceElement<const ElementT&>
{
    ErasedReferenceElement() = default;
    ErasedReferenceElement(const ElementT& e) :
        m_data{ &e }
    {}

    auto data() const -> const ElementT*
    {
        return m_data;
    }

    const ElementT* m_data{ nullptr };
};

} // namespace detail

template<typename ElementT>
class ErasedReference : private Base
{
public:
    template<typename GeneratorT, std::enable_if_t<!std::is_same<GeneratorT, ErasedReference<ElementT>>::value>* = nullptr>
    ErasedReference(GeneratorT& generator) :
        m_generator{ &generator },
        m_fetch(fetch<GeneratorT>)
    {
        generatorUpdate();
    }

private:
    friend class BaseAccess;

    using Element = ElementT;

    template<typename GeneratorT>
    static auto fetch(void* gen) -> detail::ErasedReferenceElement<Element>
    {
        rafalw_utils_assert(gen);

        auto& gen2 = *static_cast<GeneratorT*>(gen);

        if (done(gen2))
            return {};

        const auto sg = utils::scope_guard([&gen2]{
            update(gen2);
        });

        return detail::ErasedReferenceElement<Element>{ peek(gen2) };
    }

    void* m_generator;
    utils::FunctionPtr<detail::ErasedReferenceElement<Element>(void*)> m_fetch;

    detail::ErasedReferenceElement<Element> m_element;

    auto generatorDone() const -> bool
    {
        return !m_element.data();
    }

    decltype(auto) generatorPeek() const
    {
        return *m_element.data();
    }

    auto generatorUpdate() -> void
    {
        m_element = m_fetch(m_generator);
    }
};

template<typename Generator>
auto erased_reference(Generator& gen) -> ErasedReference<decltype(peek(gen))>
{
    return ErasedReference<decltype(peek(gen))>{ gen };
}

} // namespace generator
} // namespace rafalw

#endif // RAFALW_GENERATOR_ERASEDREFERENCE_HPP_
