#ifndef XTRAS_TEMPLATES_HPP_
#define XTRAS_TEMPLATES_HPP_

namespace xtras {
namespace templates {

namespace detail {

    template<template<typename...> class Element0, template<typename...> class... Elements>
    struct Chain
    {
        template<typename T>
        struct base
        {
            template<typename... Args>
            using args = Element0<Args..., typename Chain<Elements...>::template base<T>::template args<Args...>>;
        };

        template<typename... Args>
        using args = Element0<Args..., typename Chain<Elements...>::template args<Args...>>;
    };

    template<template<typename...> class Element0>
    struct Chain<Element0>
    {
        template<typename T>
        struct base
        {
            template<typename... Args>
            using args = Element0<Args..., T>;
        };

        template<typename... Args>
        using args = Element0<Args...>;
    };

    template<template<typename> class... Args>
    struct Compose;

    template<template<typename> class Arg, template<typename> class... Args>
    struct Compose<Arg, Args...>
    {
        template<typename T>
        using Result = typename Compose<Args...>::template Result<Arg<T>>;
    };

    template<>
    struct Compose<>
    {
        template<typename T>
        using Result = T;
    };

	template<typename SourceType, template<typename...> class TargetTemplate>
	struct ParamsCopy;

	template<template<typename...> class SourceTemplate, typename... SourceParams, template<typename...> class TargetTemplate>
	struct ParamsCopy<SourceTemplate<SourceParams...>, TargetTemplate>
	{
		using Result = TargetTemplate<SourceParams...>;
	};

	template<typename T>
	struct ParamsCount;

	template<template<typename...> class Template, typename... Args>
	struct ParamsCount<Template<Args...>> : public std::integral_constant<std::size_t, sizeof...(Args)> {};

	template<typename Type, template<typename...> class Template>
	struct IsInstance : public std::false_type {};

	template<template<typename...> class Template, typename... TemplateArgs>
	struct IsInstance<Template<TemplateArgs...>, Template> : public std::true_type {};

} // namespace detail

template<template<typename...> class... Elements>
using chain = detail::Chain<Elements...>;

template<typename T, template<typename> class... Args>
using compose = typename detail::Compose<Args...>::template Result<T>;

template<typename Type, template<typename...> class Template>
using is_instance = typename detail::IsInstance<Type, Template>::type;

template<typename Type, template<typename...> class Template>
inline constexpr auto is_instance_v = is_instance<Type, Template>::value;

template<typename SourceType, template<typename...> class TargetTemplate>
using params_copy = typename detail::ParamsCopy<SourceType, TargetTemplate>::Result;

template<typename T>
using params_count = typename detail::ParamsCount<T>::type;

template<typename T>
inline constexpr auto params_count_v = params_count<T>::value;

} // namespace templates
} // namespace xtras

#endif // XTRAS_TEMPLATES_HPP_
