#ifndef BEAE9345_441A_49A7_AB8A_28C3C1F463AD
#define BEAE9345_441A_49A7_AB8A_28C3C1F463AD

#include <optional>
#include <concepts>

#include "Platform.hpp"

namespace Cxx
{
  template <typename Type>
  struct Optional : public std::optional<Type>
  {
      using BaseType = std::optional<Type>;

      using BaseType::optional;
      using BaseType::operator=;

#if defined(__cpp_explicit_this_parameter) and defined(__cpp_multidimensional_subscript)
      template <typename Self, std::integral... Indexes>
      constexpr decltype(auto) operator[](this Self&& This, const Indexes... Index)
      {
        if constexpr ( requires { This->operator[](Index...); } )
        {
          return This->operator[](Index...);
        }
        else
        {
          return This.operator*().operator[](Index...);
        }
      }

#endif

#ifdef __cpp_explicit_this_parameter
      template <typename Self>
      constexpr decltype(auto) operator*(this Self&& This)
      {
        if constexpr ( requires { *This.value(); } )
        {
          return *This.value();
        }
        else if constexpr ( requires { *This.value().get(); } )
        {
          return *This.value().get();
        }
        else if constexpr ( requires { This.value().get(); } )
        {
          return This.value().get();
        }
        else if constexpr ( requires { This->operator*(); } )
        {
          return This->operator*();
        }
        else
        {
          return This.BaseType::operator*();
        }
      }

#endif

#ifdef __cpp_explicit_this_parameter
      template <typename Self>
      constexpr decltype(auto) operator->(this Self&& This)
      {
        if constexpr ( requires { This.value().operator->(); } )
        {
          return This.value();
        }
        else
        {
          return This.BaseType::operator->();
        }
      }
#endif
  };
} // namespace Cxx

#endif /* BEAE9345_441A_49A7_AB8A_28C3C1F463AD */
