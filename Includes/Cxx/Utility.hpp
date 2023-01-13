#ifndef CC4CC4EA_6005_4642_BC5F_855D9EC6C211
#define CC4CC4EA_6005_4642_BC5F_855D9EC6C211

#include "TypeTraits.hpp"

namespace Cxx::Utilities
{
  template <typename T>
  [[nodiscard]] inline constexpr decltype(auto) deep_raw_pointer_dereference(T&& pointer_or_value) noexcept
  {
    if constexpr ( requires { *pointer_or_value; } )
    {
      return deep_raw_pointer_dereference(*pointer_or_value);
    }
    else
    {
      return pointer_or_value;
    }
  }

  template <typename T>
  [[nodiscard]] inline constexpr decltype(auto) deep_smart_pointer_dereference(T&& pointer_or_value) noexcept
  {
    if constexpr ( requires { *pointer_or_value; } )
    {
      return deep_smart_pointer_dereference(*pointer_or_value);
    }
    else if constexpr ( requires { *pointer_or_value.get(); } )
    {
      return deep_smart_pointer_dereference(*pointer_or_value.get());
    }
    else
    {
      return pointer_or_value;
    }
  }
} // namespace Cxx::Utilities

#endif /* CC4CC4EA_6005_4642_BC5F_855D9EC6C211 */
