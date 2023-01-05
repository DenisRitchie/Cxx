#ifndef EDE2117E_A7D9_4EB3_892C_7B76526D6A96
#define EDE2117E_A7D9_4EB3_892C_7B76526D6A96

#include "SemanticValue.hpp"

#include <type_traits>
#include <concepts>

namespace NativeDesignPatterns
{
  namespace Details
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

    template <typename Type>
    using RemoveAllSymbols = std::remove_cv_t<std::remove_all_extents_t<std::remove_pointer_t<std::remove_cvref_t<Type>>>>;
  } // namespace Details

  template <typename Type, typename... Types>
  inline constexpr bool IsAnyOf = std::disjunction_v<std::is_same<Type, Types>...>;

  template <typename TemplateType>
  struct TemplateTraits
  {
      using Template    = Details::RemoveAllSymbols<TemplateType>;
      using ElementType = typename Details::GetElementType<Template>::Type;

      template <typename OtherType>
      using Rebind = typename Details::GetRebindAlias<Template, OtherType>::Type;
  };

  namespace Details
  {
    // clang-format off

    template <typename FactoryType>
    requires requires(const FactoryType Factory)
    {
      { Factory() } -> std::same_as<SemanticValue<typename TemplateTraits<decltype(Factory())>::ElementType>>;
    }
    struct ServiceLocatorFactoryTraits
    {
        inline static constexpr bool IsDefaultConstructible = std::is_default_constructible_v<FactoryType>;

        using ReturnType        = decltype(std::declval<FactoryType>()());
        using SemanticValueType = decltype(std::declval<FactoryType>()());
        using ValueType         = typename TemplateTraits<SemanticValueType>::ElementType;
        using ElementType       = typename TemplateTraits<SemanticValueType>::ElementType;
    };

    // clang-format on
  } // namespace Details
} // namespace NativeDesignPatterns

#endif /* EDE2117E_A7D9_4EB3_892C_7B76526D6A96 */
