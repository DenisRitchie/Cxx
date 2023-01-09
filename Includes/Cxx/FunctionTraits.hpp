// https://en.cppreference.com/w/cpp/language/parameter_pack
// https://en.cppreference.com/w/cpp/language/sizeof...
// https://en.cppreference.com/w/cpp/language/fold
// https://en.cppreference.com/w/cpp/language/function

#ifndef C2E87DC9_C92B_4119_AE64_4685318B7705
#define C2E87DC9_C92B_4119_AE64_4685318B7705

#include <functional>
#include "TypeTraits.hpp"

namespace Cxx::Traits
{
  template <typename CallableObject>
  struct FunctionTraits;

  template <typename ReturnT, typename... ArgumentTypes>
  struct FunctionTraits<ReturnT(ArgumentTypes...)>
  {
      using Object     = void;
      using Pointer    = ReturnT (*)(ArgumentTypes...);
      using Function   = std::function<ReturnT(ArgumentTypes...)>;
      using ReturnType = ReturnT;
      using Arguments  = TypeParameters::Arguments<ArgumentTypes...>;

      template <size_t Index>
      using Argument = TypeParameters::ArgumentIndexType<Index, Arguments>;

      inline static constexpr size_t ArgumentCount = sizeof...(ArgumentTypes);
      inline static constexpr bool   HasArguments  = ArgumentCount not_eq 0;
  };

#define DEFINE_FUNCTION_TRAITS(...)                                                                                 \
 template <typename ReturnType, typename... ArgumentTypes>                                                          \
 struct FunctionTraits<ReturnType (*)(ArgumentTypes...) __VA_ARGS__> : FunctionTraits<ReturnType(ArgumentTypes...)> \
 {                                                                                                                  \
 };

  DEFINE_FUNCTION_TRAITS()
  DEFINE_FUNCTION_TRAITS(noexcept)

#undef DEFINE_FUNCTION_TRAITS

#define DEFINE_FUNCTION_TRAITS(...)                                                                                             \
 template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>                                                 \
 struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) __VA_ARGS__> : FunctionTraits<ReturnType(ArgumentTypes...)> \
 {                                                                                                                              \
 };

  DEFINE_FUNCTION_TRAITS()
  DEFINE_FUNCTION_TRAITS(const)
  DEFINE_FUNCTION_TRAITS(volatile)
  DEFINE_FUNCTION_TRAITS(const volatile)

  DEFINE_FUNCTION_TRAITS(noexcept)
  DEFINE_FUNCTION_TRAITS(const noexcept)
  DEFINE_FUNCTION_TRAITS(volatile noexcept)
  DEFINE_FUNCTION_TRAITS(const volatile noexcept)

  DEFINE_FUNCTION_TRAITS(&)
  DEFINE_FUNCTION_TRAITS(const&)
  DEFINE_FUNCTION_TRAITS(volatile&)
  DEFINE_FUNCTION_TRAITS(const volatile&)

  DEFINE_FUNCTION_TRAITS(& /**/ noexcept)
  DEFINE_FUNCTION_TRAITS(const& noexcept)
  DEFINE_FUNCTION_TRAITS(volatile& noexcept)
  DEFINE_FUNCTION_TRAITS(const volatile& noexcept)

  DEFINE_FUNCTION_TRAITS(&&)
  DEFINE_FUNCTION_TRAITS(const&&)
  DEFINE_FUNCTION_TRAITS(volatile&&)
  DEFINE_FUNCTION_TRAITS(const volatile&&)

  DEFINE_FUNCTION_TRAITS(&& /**/ noexcept)
  DEFINE_FUNCTION_TRAITS(const&& noexcept)
  DEFINE_FUNCTION_TRAITS(volatile&& noexcept)
  DEFINE_FUNCTION_TRAITS(const volatile&& noexcept)

#undef DEFINE_FUNCTION_TRAITS

  template <typename FunctionType>
  struct FunctionTraits<std::function<FunctionType>> : FunctionTraits<FunctionType>
  {
  };

  template <typename CallableObject>
  struct FunctionTraits : FunctionTraits<decltype(&CallableObject::operator())>
  {
  };
} // namespace Cxx::Traits

#endif /* C2E87DC9_C92B_4119_AE64_4685318B7705 */
