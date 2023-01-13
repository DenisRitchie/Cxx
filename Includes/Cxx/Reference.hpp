#ifndef F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE
#define F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE

#include "TypeTraits.hpp"
#include "Utility.hpp"

namespace Cxx
{
  template <typename Type>
  class Reference : public std::reference_wrapper<Type>
  {
    public:
      using std::reference_wrapper<Type>::reference_wrapper;

      template <typename Self>
      constexpr decltype(auto) operator->(this Self&& self) noexcept
      {
        if constexpr ( requires { Utilities::deep_raw_pointer_dereference(self.get()).operator->(); } )
        {
          return Utilities::deep_raw_pointer_dereference(self.get());
        }
        else
        {
          return &Utilities::deep_raw_pointer_dereference(self.get());
        }
      }

      template <typename Self>
      constexpr decltype(auto) operator*(this Self&& self) noexcept
      {
        return Utilities::deep_smart_pointer_dereference(self.get());
      }
  };
} // namespace Cxx

#endif /* F7A4ABF5_1DB0_4DD2_95B1_32A3104EB8DE */
