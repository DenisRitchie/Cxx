#ifndef EDE2117E_A7D9_4EB3_892C_7B76526D6A96
#define EDE2117E_A7D9_4EB3_892C_7B76526D6A96

#include <concepts>

namespace Cxx::Traits
{
  namespace TypeParameters::inline V1
  {
    template <typename Type>
    struct GetFirstParameter;

    template <template <typename, typename...> class TemplateType, typename FirstType, typename... RestType>
    struct GetFirstParameter<TemplateType<FirstType, RestType...>>
    {
        using Type = FirstType;
    };

    template <typename NewFirstType, typename Type>
    struct ReplaceFirstParameter;

    template <typename NewFirstType, template <typename, typename...> class TemplateType, typename FirstType, typename... RestType>
    struct ReplaceFirstParameter<NewFirstType, TemplateType<FirstType, RestType...>>
    {
        using Type = TemplateType<NewFirstType, RestType...>;
    };

    template <typename TemplateType, typename = void>
    struct GetElementType
    {
        using Type = typename GetFirstParameter<TemplateType>::Type;
    };

    template <typename TemplateType>
    struct GetElementType<TemplateType, std::void_t<typename TemplateType::ElementType>>
    {
        using Type = typename TemplateType::ElementType;
    };

    template <typename TemplateType, typename OtherType, typename = void>
    struct GetRebindAlias
    {
        using Type = typename ReplaceFirstParameter<OtherType, TemplateType>::Type;
    };

    template <typename TemplateType, typename OtherType>
    struct GetRebindAlias<TemplateType, OtherType, std::void_t<typename TemplateType::template Rebind<OtherType>>>
    {
        using Type = typename TemplateType::template Rebind<OtherType>;
    };
  } // namespace TypeParameters::inline V1

  namespace TypeParameters::inline V2
  {
    template <typename... Types>
    struct Arguments;

    template <size_t Index, typename ArgumentPack>
    struct ArgumentIndex;

    template <size_t Index>
    struct ArgumentIndex<Index, Arguments<>>
    {
        static_assert(AlwaysFalse<std::integral_constant<size_t, Index>>, "Argument index out of bounds");
    };

    template <typename FirstType, typename... RestType>
    struct ArgumentIndex<0, Arguments<FirstType, RestType...>>
    {
        using ArgumentType = FirstType;
    };

    template <size_t Index, typename FirstType, typename... RestType>
    struct ArgumentIndex<Index, Arguments<FirstType, RestType...>> : ArgumentIndex<Index - 1, Arguments<RestType...>>
    {
    }; // Recursive ArgumentIndex Definition

    template <size_t Index, typename ArgumentPack>
    using ArgumentIndexType = typename ArgumentIndex<Index, ArgumentPack>::ArgumentType;

    template <typename... Types>
    struct Arguments
    {
        inline static constexpr size_t ArgumentCount = sizeof...(Types);

        template <size_t Index>
        using ArgumentIndex = ArgumentIndexType<Index, Arguments<Types...>>;
    };

    namespace Details
    {
      template <typename TemplateType>
      struct TemplateArguments
      {
          using Arguments = V2::Arguments<>;
      };

      template <template <typename...> class TemplateType, typename... Types>
      struct TemplateArguments<TemplateType<Types...>>
      {
          using Arguments = V2::Arguments<Types...>;
      };
    } // namespace Details

    template <typename TemplateType>
    using TemplateArguments = typename Details::TemplateArguments<TemplateType>::Arguments;

    template <size_t I, typename T>
    struct Arg
    {
        using Type = T;

        inline static constexpr size_t Index = I;
    };

    template <typename TemplateType, typename... ArgIndex>
    struct ReplaceArguments;

    template <template <typename...> class TemplateType, typename... Types, size_t... ArgIndex, typename... ArgType>
    struct ReplaceArguments<TemplateType<Types...>, Arg<ArgIndex, ArgType>...>
    {
        using Type = void; // TODO: Crear Implementación
    };

  } // namespace TypeParameters::inline V2

  template <typename>
  inline constexpr bool AlwaysFalse = false; // false value attached to a dependent name (for static_assert)

  template <size_t Size>
  inline constexpr size_t NumberOfArguments = Size;

  template <typename Type, typename... Types>
  inline constexpr bool IsAnyOf = std::disjunction_v<std::is_same<Type, Types>...>;

  template <typename Type>
  using RemoveAllSymbols = std::remove_cv_t<std::remove_all_extents_t<std::remove_pointer_t<std::remove_cvref_t<Type>>>>;

  template <typename TemplateType>
  struct TemplateTraits
  {
      using Template       = RemoveAllSymbols<TemplateType>;
      using TypeParameters = TypeParameters::TemplateArguments<Template>;
      using ElementType    = TypeParameters::template ArgumentIndex<0>;

      template <size_t Index>
      using TypeParameter = TypeParameters::template ArgumentIndex<Index>;

      //   template <typename Type>
      //   using Rebind = typename TypeParameters::GetRebindAlias<Template, Type>::Type;
  };
} // namespace Cxx::Traits

#endif /* EDE2117E_A7D9_4EB3_892C_7B76526D6A96 */
