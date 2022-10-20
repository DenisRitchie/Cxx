#ifndef D3B300A4_FC34_4A64_BEC3_75BB932C037F
#define D3B300A4_FC34_4A64_BEC3_75BB932C037F

#include <any>
#include <type_traits>

namespace NativeDesignPatterns
{
  template <typename BaseType>
  class SemanticValue
  {
    public:
      using type            = BaseType;
      using element_type    = BaseType;
      using value_type      = std::remove_cv_t<BaseType>;
      using reference       = element_type&;
      using pointer         = element_type*;
      using const_reference = const element_type&;
      using const_pointer   = const element_type*;

      template <typename DerivedType>
      constexpr SemanticValue(DerivedType&& value) noexcept
        : object{ std::forward<DerivedType>(value) }
        , getter{ [](std::any& object) -> BaseType&
                  {
                    return std::any_cast<DerivedType&>(object);
                  } }
      {
      }

      template <typename Self>
      inline constexpr auto operator*(this Self&& self) noexcept
          -> std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const_reference, reference>
      {
        return self.getter(const_cast<std::any&>(self.object));
      }

      template <typename Self>
      inline constexpr auto operator->(this Self&& self) noexcept //
          -> std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const_pointer, pointer>
      {
        return &self.getter(const_cast<std::any&>(self.object));
      }

    private:
      std::any object;
      reference (*getter)(std::any&);
  };
} // namespace NativeDesignPatterns

#endif /* D3B300A4_FC34_4A64_BEC3_75BB932C037F */
