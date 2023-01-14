#ifndef C35229B1_A65B_4639_97F8_E0C58134BB4F
#define C35229B1_A65B_4639_97F8_E0C58134BB4F

#include <string>
#include <string_view>
#include <iterator>
#include <locale>
#include <utility>

#include "FunctionTraits.hpp"
#include "Coroutines/Generator.hpp"

namespace Cxx
{
  template <typename ValueType, typename ExtensionType>
  constexpr auto operator->*(ValueType&& Value, ExtensionType&& Extension) -> std::invoke_result_t<ExtensionType, ValueType>
  {
    return Extension(Value);
  }
} // namespace Cxx

namespace Cxx::LINQ::QuerySyntax
{

} // namespace Cxx::LINQ::QuerySyntax

namespace Cxx::LINQ::MethodSyntax
{
  namespace PropertyDetails
  {
    template <std::ranges::range RangeType>
    struct ToImplementation
    {
        // * TODO: Crear un iterador personalizado para los distintos métodos de inserción
        // * TODO: Detectar si tipo de end() es diferente de begin() y crear un iterador común en ese caso
        constexpr auto operator()(auto&& value) const -> RangeType
        {
          RangeType result;
          std::copy(value.begin(), value.end(), std::back_inserter(result));
          return result;
        }
    };

    struct UpperCaseImplementation
    {
        template <typename CharType, typename TraitType, typename AllocatorType>
        constexpr auto&& operator()(const std::basic_string<CharType, TraitType, AllocatorType>& value) const noexcept
        {
          CharType*   letter      = const_cast<CharType*>(value.data());
          const auto& ctype_facet = std::use_facet<std::ctype_byname<char>>(std::locale());

          for ( ; *letter != '\0'; ++letter )
          {
            *letter = static_cast<CharType>(ctype_facet.toupper(*letter));
          }

          return value;
        }
    };

    struct LowerCaseImplementation
    {
        template <typename CharType, typename TraitType, typename AllocatorType>
        constexpr auto&& operator()(const std::basic_string<CharType, TraitType, AllocatorType>& value) const noexcept
        {
          CharType*   letter      = const_cast<CharType*>(value.data());
          const auto& ctype_facet = std::use_facet<std::ctype_byname<char>>(std::locale());

          for ( ; *letter != '\0'; ++letter )
          {
            *letter = static_cast<CharType>(ctype_facet.tolower(*letter));
          }

          return value;
        }
    };

    struct LengthImplementation
    {
        // clang-format off

        // * TODO: Crear implementación para obtener de diferentes maneras la cantidad de items de un array
        template <typename Type>
        requires requires(Cxx::Traits::RemoveAllSymbols<Type> value)
        {
          { value.size() };
        }
        constexpr auto operator()(Type&& value) const noexcept
        {
          return value.size();
        }

        // clang-format on
    };

    struct BooleanStringImplementation
    {
        inline static constexpr std::string_view BooleanString(const bool value) noexcept
        {
          return value ? "True" : "False";
        }

        template <typename Type>
        constexpr std::string_view operator()(Type&& value) const noexcept
        {
          if constexpr ( std::is_pointer_v<std::remove_cvref_t<Type>> and Traits::IsAnyOf<Cxx::Traits::RemoveAllSymbols<Type>, char, int8_t, uint8_t, wchar_t, char8_t, char16_t, char32_t> )
          {
            return BooleanString(*value != 0);
          }
          else if constexpr ( std::is_array_v<std::remove_cvref_t<Type>> and Traits::IsAnyOf<Cxx::Traits::RemoveAllSymbols<Type>, char, int8_t, uint8_t, wchar_t, char8_t, char16_t, char32_t> )
          {
            return BooleanString(value[0] != 0);
          }
          else if constexpr ( std::is_arithmetic_v<std::remove_cvref_t<Type>> )
          {
            return BooleanString(!!value);
          }
          else if constexpr ( requires { value.operator bool(); } )
          {
            return BooleanString(value.operator bool());
          }
          else if constexpr ( requires { value.operator!(); } )
          {
            return BooleanString(!value.operator!());
          }
          else if constexpr ( requires { value.empty(); } )
          {
            return BooleanString(!value.empty());
          }
          else if constexpr ( requires { value.size(); } )
          {
            return BooleanString(value.size() != 0);
          }
          else if constexpr ( requires { std::size(value); } )
          {
            return BooleanString(std::size(value) != 0);
          }
          else if constexpr ( requires { value.has_value(); } )
          {
            return value.has_value();
          }
          else if constexpr ( std::is_pointer_v<std::remove_cvref_t<Type>> and requires { *value; } )
          {
            if ( value != nullptr )
            {
              return this->operator()(*value);
            }
            else
            {
              return BooleanString(false);
            }
          }
          else if constexpr ( requires { value != nullptr; } )
          {
            return BooleanString(value != nullptr);
          }
          else
          {
            return BooleanString(true);
          }
        }
    };
  } // namespace PropertyDetails

  template <std::ranges::range RangeType>
  inline constexpr PropertyDetails::ToImplementation<RangeType> To;
  inline constexpr PropertyDetails::LengthImplementation        Count;
  inline constexpr PropertyDetails::LengthImplementation        Size;
  inline constexpr PropertyDetails::LengthImplementation        Length;
  inline constexpr PropertyDetails::UpperCaseImplementation     UpperCase;
  inline constexpr PropertyDetails::LowerCaseImplementation     LowerCase;
  inline constexpr PropertyDetails::BooleanStringImplementation BooleanString;

  template <std::ranges::range RangeType>
  auto As()
  {
    return [](auto&& value)
    {
      return RangeType(value.begin(), value.end());
    };
  }

  template <typename FunctionType>
  auto Select(FunctionType&& Selector)
  {
    using return_type = Coroutines::Generator<typename Traits::FunctionTraits<FunctionType>::ReturnType>;

    return [&](std::ranges::range auto&& values) -> return_type
    {
      for ( auto&& value : values )
      {
        co_yield Selector(value);
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
    return [](auto&& value)
    {
      if constexpr ( requires { value.to_string(); } )
      {
        return value.to_string();
      }
      else if constexpr ( requires { value.ToString(); } )
      {
        return value.ToString();
      }
      else
      {
        return std::to_string(value);
      }
    };
  }

} // namespace Cxx::LINQ::MethodSyntax

#endif /* C35229B1_A65B_4639_97F8_E0C58134BB4F */
