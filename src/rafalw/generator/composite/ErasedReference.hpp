#ifndef RAFALW_GENERATOR_ERASEDREFERENCE_HPP_
#define RAFALW_GENERATOR_ERASEDREFERENCE_HPP_

#include <rafalw/generator/Base.hpp>

inline namespace rafalw {
namespace generator {

template<typename ElementT>
class ErasedReference : private Base
{
public:
    template<typename GeneratorT, std::enable_if_t<!std::is_same<GeneratorT, ErasedReference<ElementT>>::value>* = nullptr>
    ErasedReference(GeneratorT& generator)
    {
        static_assert(sizeof(ImplBase) == sizeof(Impl<GeneratorT>));
        new (m_implStorage) Impl<GeneratorT>{ &generator };
    }

private:
    friend class BaseAccess;

    using Element = ElementT;

    class ImplBase
    {
    public:
        ImplBase(void* generator) :
            m_generator{ generator }
        {}

        virtual auto done() const -> bool = 0;
        virtual auto peek() const -> Element = 0;
        virtual auto update() -> void = 0;

    protected:
        void* m_generator;
    };

    template<typename GeneratorT>
    class Impl : public ImplBase
    {
    public:
        using ImplBase::ImplBase;

        auto done() const -> bool override
        {
            return generator::done(get());
        }

        auto peek() const -> Element override
        {
            return generator::peek(get());
        }

        auto update() -> void override
        {
            generator::update(get());
        }

    private:
        auto get() const -> const GeneratorT&
        {
            return *static_cast<const GeneratorT*>(ImplBase::m_generator);
        }

        auto get() -> GeneratorT&
        {
            return *static_cast<GeneratorT*>(ImplBase::m_generator);
        }
    };

    alignas (ImplBase) std::uint8_t m_implStorage[sizeof(ImplBase)];

    auto generatorDone() const -> bool
    {
        return impl().done();
    }

    decltype(auto) generatorPeek() const
    {
        return impl().peek();
    }

    auto generatorUpdate() -> void
    {
        impl().update();
    }

    auto impl() -> ImplBase&
    {
        return *reinterpret_cast<ImplBase*>(&m_implStorage[0]);
    }

    auto impl() const -> const ImplBase&
    {
        return *reinterpret_cast<const ImplBase*>(&m_implStorage[0]);
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
