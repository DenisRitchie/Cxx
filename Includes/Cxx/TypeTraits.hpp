#ifndef EDE2117E_A7D9_4EB3_892C_7B76526D6A96
#define EDE2117E_A7D9_4EB3_892C_7B76526D6A96

#include <concepts>

namespace Cxx::Traits::TypeParameters
{
  namespace [[deprecated]] V1
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
  } // namespace V1
} // namespace Cxx::Traits::TypeParameters

namespace Cxx::Traits::TypeParameters::inline V2
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

  namespace Details
  {
    template <typename TemplateType, typename = void>
    struct ReplaceArgumentPack;

    template <template <typename...> class TemplateType, typename... Types, size_t... Index>
    struct ReplaceArgumentPack<TemplateType<Types...>, std::index_sequence<Index...>>
    {
        using Args = Arguments<Arg<Index, Types>...>;
    };

    struct NotFound
    {
    };

    template <size_t SearchIndex, typename ReplaceArguments, typename = void>
    struct FindIndexImpl;

    template <size_t SearchIndex, typename ReplaceArguments>
    struct FindIndexImpl<SearchIndex, ReplaceArguments, std::false_type>
    {
        using Type = NotFound;

        inline static constexpr bool Exists = false;
    };

    template <size_t SearchIndex, typename ReplaceArguments>
    struct FindIndexImpl<SearchIndex, ReplaceArguments, std::true_type>
    {
        using Type = typename ReplaceArguments::template ArgumentIndex<SearchIndex>;

        inline static constexpr bool Exists = true;
    };

    template <size_t SearchIndex, class ReplaceArguments, class IndexSequence>
    struct FindIndex;

    template <size_t SearchIndex, template <typename...> class ReplaceArguments, typename... ReplaceArgType, size_t... ReplaceArgIndex>
    struct FindIndex<SearchIndex, ReplaceArguments<ReplaceArgType...>, std::index_sequence<ReplaceArgIndex...>>
    {
        using Impl = FindIndexImpl<SearchIndex, ReplaceArguments<ReplaceArgType...>, std::bool_constant<((SearchIndex == ReplaceArgIndex) || ...)>>;
        using Type = typename Impl::Type;

        inline static constexpr bool Exists = Impl::Exists;
    };

    template <typename TemplateType, typename TemplateIndices, typename ReplaceArgs>
    struct ReplaceArgumentsImpl;

    template <template <typename...> class TemplateType, typename... TypeParameters, size_t... TemplateArgIndex, size_t... ReplaceArgIndex, typename... ReplaceArgType>
    struct ReplaceArgumentsImpl<TemplateType<TypeParameters...>, std::index_sequence<TemplateArgIndex...>, Arguments<Arg<ReplaceArgIndex, ReplaceArgType>...>>
    {
        using TemplateArgs = ReplaceArgumentPack<TemplateType<TypeParameters...>, std::index_sequence_for<TypeParameters...>>::Args;
        using ReplaceArgs  = Arguments<Arg<ReplaceArgIndex, ReplaceArgType>...>;

        // clang-format off
        using Type = TemplateType
        <
            std::conditional_t
            <
                FindIndex<TemplateArgIndex, ReplaceArgs, std::index_sequence_for<ReplaceArgType...>>::Exists,
                typename FindIndex<TemplateArgIndex, ReplaceArgs, std::index_sequence_for<ReplaceArgType...>>::Type,
                typename TemplateArgs::template ArgumentIndex<TemplateArgIndex>::Type
            >...
        >;
        // clang-format on
    };
  } // namespace Details

  template <typename TemplateType, typename... ArgIndex>
  struct ReplaceArguments;

  template <template <typename...> class TemplateType, typename... Types, size_t... ArgIndex, typename... ArgType>
  struct ReplaceArguments<TemplateType<Types...>, Arg<ArgIndex, ArgType>...>
  {
      static_assert(
        ((Arg<ArgIndex, ArgType>::Index >= 0 && Arg<ArgIndex, ArgType>::Index < sizeof...(Types)) && ...), //
        "All argument indices must be greater than or equal to zero and less than the maximum number of arguments of type template"
      );

      using _Impl        = Details::ReplaceArgumentsImpl<TemplateType<Types...>, std::index_sequence_for<Types...>, Arguments<Arg<ArgIndex, ArgType>...>>;
      using TemplateArgs = _Impl::TemplateArgs;
      using ReplaceArgs  = _Impl::ReplaceArgs;
      using Type         = _Impl::Type;
  };
} // namespace Cxx::Traits::TypeParameters::inline V2

namespace Cxx::Traits
{
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
