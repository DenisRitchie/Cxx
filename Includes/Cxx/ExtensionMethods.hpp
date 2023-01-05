#ifndef C35229B1_A65B_4639_97F8_E0C58134BB4F
#define C35229B1_A65B_4639_97F8_E0C58134BB4F

#include "Generator.hpp"
#include "Details/TypeTraits.hpp"

#include <string>
#include <string_view>
#include <iterator>
#include <locale>
#include <utility>

template <typename ValueType, typename ExtensionType>
constexpr auto operator->*(ValueType&& Value, ExtensionType&& Extension) -> decltype(std::declval<ExtensionType>()(std::declval<ValueType>()))
{
  return Extension(Value);
}

namespace NativeDesignPatterns::LINQ::QuerySyntax
{

} // namespace NativeDesignPatterns::LINQ::QuerySyntax

namespace NativeDesignPatterns::LINQ::MethodSyntax
{
  namespace PropertyDetails
  {
    template <std::ranges::range RangeType>
    struct ToImplementation
    {
        constexpr auto operator()(auto&& Value) const -> RangeType
        {
          RangeType Result;
          std::copy(Value.begin(), Value.end(), std::back_inserter(Result));
          return Result;
        }
    };

    struct UpperCaseImplementation
    {
        template <typename CharType, typename TraitType, typename AllocatorType>
        constexpr auto&& operator()(const std::basic_string<CharType, TraitType, AllocatorType>& Value) const noexcept
        {
          auto Letter = const_cast<CharType*>(Value.data());

          while ( *Letter != '\0' )
          {
            *Letter = static_cast<CharType>(std::toupper(*Letter, std::locale()));
            ++Letter;
          }

          return Value;
        }
    };

    struct LowerCaseImplementation
    {
        template <typename CharType, typename TraitType, typename AllocatorType>
        constexpr auto&& operator()(const std::basic_string<CharType, TraitType, AllocatorType>& Value) const noexcept
        {
          auto Letter = const_cast<CharType*>(Value.data());

          while ( *Letter != '\0' )
          {
            *Letter = static_cast<CharType>(std::tolower(*Letter, std::locale()));
            ++Letter;
          }

          return Value;
        }
    };

    struct LengthImplementation
    {
        // clang-format off

        template <typename Type>
        requires requires(NativeDesignPatterns::Details::RemoveAllSymbols<Type> Value)
        {
          { Value.size() };
        }
        constexpr auto operator()(Type&& Value) const noexcept
        {
          return Value.size();
        }

        // clang-format on
    };

    struct BooleanStringImplementation
    {
        inline static constexpr std::string_view BooleanString(const bool Value) noexcept
        {
          return Value ? "True" : "False";
        }

        template <typename Type>
        constexpr std::string_view operator()(Type&& Value) const noexcept
        {
          if constexpr ( std::is_pointer_v<std::remove_cvref_t<Type>> and IsAnyOf<NativeDesignPatterns::Details::RemoveAllSymbols<Type>, char, int8_t, uint8_t, wchar_t, char8_t, char16_t, char32_t> )
          {
            return BooleanString(*Value != 0);
          }
          else if constexpr ( std::is_array_v<std::remove_cvref_t<Type>> and IsAnyOf<NativeDesignPatterns::Details::RemoveAllSymbols<Type>, char, int8_t, uint8_t, wchar_t, char8_t, char16_t, char32_t> )
          {
            return BooleanString(Value[0] != 0);
          }
          else if constexpr ( std::is_arithmetic_v<std::remove_cvref_t<Type>> )
          {
            return BooleanString(!!Value);
          }
          else if constexpr ( requires { Value.operator bool(); } )
          {
            return BooleanString(Value.operator bool());
          }
          else if constexpr ( requires { Value.operator!(); } )
          {
            return BooleanString(!Value.operator!());
          }
          else if constexpr ( requires { Value.empty(); } )
          {
            return BooleanString(!Value.empty());
          }
          else if constexpr ( requires { Value.size(); } )
          {
            return BooleanString(Value.size() != 0);
          }
          else if constexpr ( requires { std::size(Value); } )
          {
            return BooleanString(std::size(Value) != 0);
          }
          else if constexpr ( requires { Value.has_value(); } )
          {
            return Value.has_value();
          }
          else if constexpr ( std::is_pointer_v<std::remove_cvref_t<Type>> and requires { *Value; } )
          {
            if ( Value != nullptr )
            {
              return operator()(*Value);
            }
            else
            {
              return BooleanString(false);
            }
          }
          else if constexpr ( requires { Value != nullptr; } )
          {
            return BooleanString(Value != nullptr);
          }
          else
          {
            return BooleanString(true);
          }
        }
    };
  } // namespace PropertyDetails

  inline constexpr PropertyDetails::LengthImplementation        Count;
  inline constexpr PropertyDetails::LengthImplementation        Size;
  inline constexpr PropertyDetails::LengthImplementation        Length;
  inline constexpr PropertyDetails::UpperCaseImplementation     UpperCase;
  inline constexpr PropertyDetails::LowerCaseImplementation     LowerCase;
  inline constexpr PropertyDetails::BooleanStringImplementation BooleanString;

  template <std::ranges::range RangeType>
  inline constexpr PropertyDetails::ToImplementation<RangeType> To;

  template <std::ranges::range RangeType>
  auto As()
  {
    return []<typename Type>(Type&& Value)
    {
      return RangeType(Value.begin(), Value.end());
    };
  }

  template <typename FunctionType>
  auto Select(FunctionType&& Selector)
  {
    return [&]<std::ranges::range RangeType>(RangeType&& Values) // clang-format off
    -> Generator<decltype(
        std::declval<FunctionType>()(
          std::declval<typename TemplateTraits<RangeType>::ElementType>()
        )
       )> // clang-format on
    {
      for ( auto&& Value : Values )
      {
        co_yield Selector(Value);
      }
    };
  }

  template <typename FunctionType>
  auto Map(FunctionType&& Function)
  {
    return Select(Function);
  }

  auto ToString()
  {
    return [](auto&& Value)
    {
      if constexpr ( requires { Value.to_string(); } )
      {
        return Value.to_string();
      }
      else if constexpr ( requires { Value.ToString(); } )
      {
        return Value.ToString();
      }
      else
      {
        return std::to_string(Value);
      }
    };
  }

} // namespace NativeDesignPatterns::LINQ::MethodSyntax

#endif /* C35229B1_A65B_4639_97F8_E0C58134BB4F */
