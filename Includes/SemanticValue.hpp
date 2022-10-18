#ifndef D3B300A4_FC34_4A64_BEC3_75BB932C037F
#define D3B300A4_FC34_4A64_BEC3_75BB932C037F

#include <any>

namespace NativeDesignPatterns
{
  template <typename BaseType>
  class SemanticValue
  {
    public:
      template <typename DerivedType>
      constexpr SemanticValue(DerivedType&& Value) noexcept
        : Object{ std::forward<DerivedType>(Value) }
        , Getter{ [](std::any& Object) -> BaseType&
                  {
                    return std::any_cast<DerivedType&>(Object);
                  } }
      {
      }

      template <typename Self>
      constexpr decltype(auto) operator*(this Self&& This) noexcept
      {
        return This.Getter(This.Object);
      }

      template <typename Self>
      constexpr decltype(auto) operator->(this Self&& This) noexcept
      {
        return &This.Getter(This.Object);
      }

    private:
      mutable std::any Object;
      BaseType& (*Getter)(std::any&);
  };
} // namespace NativeDesignPatterns

#endif /* D3B300A4_FC34_4A64_BEC3_75BB932C037F */
