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
      constexpr decltype(auto) operator*(this Self&& This) noexcept
      {
        if constexpr // clang-format off
        (
          requires(const Type Value)
          {
            typename Type::type;
            std::is_reference_v<decltype(Value.get())>;
          }
        ) // clang-format on
        {
          return This.BaseType::operator*().get();
        }
        else
        {
          return This.BaseType::operator*();
        }
      }

      template <typename Self>
      constexpr decltype(auto) operator->(this Self&& This) noexcept
      {
        return This.BaseType::operator*();
      }
  };
} // namespace NativeDesignPatterns

#endif /* BEAE9345_441A_49A7_AB8A_28C3C1F463AD */
