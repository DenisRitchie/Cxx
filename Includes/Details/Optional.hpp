#ifndef BEAE9345_441A_49A7_AB8A_28C3C1F463AD
#define BEAE9345_441A_49A7_AB8A_28C3C1F463AD

#include <optional>
#include <concepts>
#include <type_traits>

namespace NativeDesignPatterns
{
  template <typename Type>
  struct Optional : public std::optional<Type>
  {
      using BaseType = std::optional<Type>;

      using BaseType::optional;
      using BaseType::operator=;

      template <typename Self>
      constexpr auto&& operator*(this Self&& This) noexcept
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
        else
        {
          return This.value();
        }
      }

      template <typename Self>
      constexpr auto&& operator->(this Self&& This) noexcept
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
  };
} // namespace NativeDesignPatterns

#endif /* BEAE9345_441A_49A7_AB8A_28C3C1F463AD */
