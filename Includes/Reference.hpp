#ifndef F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE
#define F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE

#include <type_traits>

namespace NativeDesignPatterns
{
  template <typename Type>
  struct Reference : public std::reference_wrapper<Type>
  {
      using BaseType = std::reference_wrapper<Type>;
      using BaseType::reference_wrapper;

      template <typename Self>
      constexpr decltype(auto) operator->(this Self&& This) noexcept
      {
        return &This.get();
      }
  };
} // namespace NativeDesignPatterns

#endif /* F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE */
