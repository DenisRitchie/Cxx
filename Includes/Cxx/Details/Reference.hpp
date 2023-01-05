#ifndef F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE
#define F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE

#include <type_traits>

namespace NativeDesignPatterns
{
  template <typename Type>
  class Reference : public std::reference_wrapper<Type>
  {
    public:
      using ValueType = std::remove_cv_t<Type>;
      using BaseType  = std::reference_wrapper<Type>;
      using BaseType::reference_wrapper;

    private:
      inline static constexpr bool HasMemberPointerAccess = requires(const ValueType Value)
      {
        { Value.operator->() };
      };

    public:
      // clang-format off
      template <typename Self>
      constexpr auto operator->(this Self&& This) noexcept
        -> std::conditional_t<HasMemberPointerAccess,
            std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const ValueType&, ValueType&>,
            std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const ValueType*, ValueType*>>
      // clang-format on
      {
        if constexpr ( requires { This.get().operator->(); } )
        {
          return This.get();
        }
        else
        {
          return reinterpret_cast<std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const ValueType*, ValueType*>>(
              std::addressof(const_cast<ValueType&>(This.get()))
          );
        }
      }
  };
} // namespace NativeDesignPatterns

#endif /* F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE */
