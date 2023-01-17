#ifndef EDE2117E_A7D9_4EB3_892C_7B76526D6A96
#define EDE2117E_A7D9_4EB3_892C_7B76526D6A96

#include <concepts>

namespace Cxx::Traits::TypeParameters
{
  namespace [[deprecated("Use namespace Cxx::Traits::TypeParameters::inline V2")]] V1
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
    struct NotFound
    {
    };

    template <bool ExistsIndex, size_t ReplaceIndex, typename ReplaceArguments>
    struct FindIndexResult;

    template <size_t ReplaceIndex, typename ReplaceArguments>
    struct FindIndexResult<false, ReplaceIndex, ReplaceArguments>
    {
        using Type = NotFound;
    };

    template <size_t ReplaceIndex, template <typename...> class ReplaceArguments, typename... ReplaceArgType>
    struct FindIndexResult<true, ReplaceIndex, ReplaceArguments<ReplaceArgType...>>
    {
        using Type = typename ReplaceArguments<ReplaceArgType...>::template ArgumentIndex<ReplaceIndex>::Type;
    };

    template <size_t SearchIndex, class ReplaceArguments, class ReplaceArgumentIndices>
    struct FindIndex;

    template <size_t TemplateSearchIndex, template <typename...> class ReplaceArguments, typename... ReplaceArgType, size_t... ReplaceArgIndex>
    struct FindIndex<TemplateSearchIndex, ReplaceArguments<ReplaceArgType...>, std::index_sequence<ReplaceArgIndex...>>
    {
        inline static constexpr size_t TemplateIndex = TemplateSearchIndex;
        inline static constexpr size_t IndexNotFound = static_cast<size_t>(-1);

        struct ValueIndex
        {
            constexpr ValueIndex(const size_t index, const size_t replace_index) noexcept
              : Index{ index }
              , ReplaceIndex{ replace_index }
            {
            }

            size_t Index;
            size_t ReplaceIndex;
        };

        inline static consteval size_t SearchIndex() noexcept
        {
          // clang-format off
          for ( const auto& [Index, ReplaceIndex] : std::initializer_list<ValueIndex> {
                  ValueIndex
                  {
                    ReplaceArgIndex,
                    ReplaceArguments<ReplaceArgType...>::template ArgumentIndex<ReplaceArgIndex>::Index
                  } ...
          } )
          // clang-format on
          {
            if ( ReplaceIndex == TemplateSearchIndex )
            {
              return Index;
            }
          }

          return IndexNotFound;
        }

        inline static constexpr bool Exists = SearchIndex() != IndexNotFound;

        using Type = typename FindIndexResult<Exists, SearchIndex(), ReplaceArguments<ReplaceArgType...>>::Type;
    };

    template <typename TemplateType, typename TemplateIndices, typename ReplaceArgs>
    struct ReplaceArgumentsImpl;

    template <template <typename...> class TemplateType, typename... TypeParameters, size_t... TemplateArgIndex, size_t... ReplaceArgIndex, typename... ReplaceArgType>
    struct ReplaceArgumentsImpl<TemplateType<TypeParameters...>, std::index_sequence<TemplateArgIndex...>, Arguments<Arg<ReplaceArgIndex, ReplaceArgType>...>>
    {
        using FindIndexArgs = Arguments<FindIndex<TemplateArgIndex, Arguments<Arg<ReplaceArgIndex, ReplaceArgType>...>, std::index_sequence_for<ReplaceArgType...>>...>;
        using TemplateArgs  = Arguments<Arg<TemplateArgIndex, TypeParameters>...>;
        using ReplaceArgs   = Arguments<Arg<ReplaceArgIndex, ReplaceArgType>...>;

        using Type = TemplateType<std::conditional_t<FindIndexArgs::template ArgumentIndex<TemplateArgIndex>::Exists, typename FindIndexArgs::template ArgumentIndex<TemplateArgIndex>::Type, TypeParameters>...>;
    };

    template <typename Arguments, typename Indices>
    struct ArgPackImpl;

    template <typename... Type, size_t... Index>
    struct ArgPackImpl<Arguments<Type...>, std::index_sequence<Index...>>
    {
        using Types = Arguments<Arg<Index, Type>...>;
    };
  } // namespace Details

  template <typename... Types>
  using ArgPack = typename Details::ArgPackImpl<Arguments<Types...>, std::index_sequence_for<Types...>>::Types;

  template <typename TemplateType, typename ArgPack, typename... ArgIndex>
  struct ReplaceArguments;

  template <template <typename...> class TemplateType, typename... Types, size_t ArgIndex0, typename ArgType0, size_t... ArgIndex, typename... ArgType>
  struct ReplaceArguments<TemplateType<Types...>, Arg<ArgIndex0, ArgType0>, Arg<ArgIndex, ArgType>...> : Details::ReplaceArgumentsImpl<TemplateType<Types...>, std::index_sequence_for<Types...>, Arguments<Arg<ArgIndex0, ArgType0>, Arg<ArgIndex, ArgType>...>>
  {
      inline static consteval bool CheckArgs()
      {
        return (Arg<ArgIndex0, ArgType0>::Index >= 0 && Arg<ArgIndex0, ArgType0>::Index < sizeof...(Types)) //
            && ((Arg<ArgIndex, ArgType>::Index >= 0 && Arg<ArgIndex, ArgType>::Index < sizeof...(Types)) && ...);
      }

      static_assert(CheckArgs(), "All argument indices must be greater than or equal to zero and less than the maximum number of arguments of type template");
  };

  template <template <typename...> class TemplateType, typename... Types, size_t... ArgIndex, typename... ArgType>
  struct ReplaceArguments<TemplateType<Types...>, Arguments<Arg<ArgIndex, ArgType>...>> : ReplaceArguments<TemplateType<Types...>, Arg<ArgIndex, ArgType>...>
  {
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

      template <typename... Types>
      using Rebind = typename Cxx::Traits::TypeParameters::ReplaceArguments<Template, Cxx::Traits::TypeParameters::ArgPack<Types...>>::Type;
  };

  template <class Type>
  struct Choice
  {
      Type Strategy = Type{};
      bool NoThrow  = false;
  };
} // namespace Cxx::Traits

#endif /* EDE2117E_A7D9_4EB3_892C_7B76526D6A96 */
