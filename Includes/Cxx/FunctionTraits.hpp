// https://en.cppreference.com/w/cpp/language/parameter_pack
// https://en.cppreference.com/w/cpp/language/sizeof...
// https://en.cppreference.com/w/cpp/language/fold
// https://en.cppreference.com/w/cpp/language/function

#ifndef C2E87DC9_C92B_4119_AE64_4685318B7705
#define C2E87DC9_C92B_4119_AE64_4685318B7705

#include "TypeTraits.hpp"
#include <functional>

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
      using Arguments  = TypeParameters::Arguments<ArgumentTypes...>;
      using ReturnType = ReturnT;

      template <size_t Index>
      using Argument = TypeParameters::ArgumentIndexType<Index, Arguments>;

      inline static constexpr size_t ArgumentCount            = sizeof...(ArgumentTypes);
      inline static constexpr bool   HasArguments             = ArgumentCount not_eq 0;
      inline static constexpr bool   IsStdFunction            = false; /* Example: std::function<>                               */
      inline static constexpr bool   IsObjectFunction         = false; /* Example: std::less<>, Lambdas                          */
      inline static constexpr bool   IsPointerFunction        = false; /* Example: void(*)(Args...)                              */
      inline static constexpr bool   IsMemberFunction         = false; /* Example: void (Object::*)()                            */
      inline static constexpr bool   IsConst                  = false; /* Example: void (Object::*)() const                      */
      inline static constexpr bool   IsVolatile               = false; /* Example: void (Object::*)() volatile                   */
      inline static constexpr bool   IsConstVolatile          = false; /* Example: void (Object::*)() const volatile             */
      inline static constexpr bool   IsNoexcept               = false; /* Example: void (Object::*)() noexcept                   */
      inline static constexpr bool   IsConstNoexcept          = false; /* Example: void (Object::*)() const noexcept             */
      inline static constexpr bool   IsVolatileNoexcept       = false; /* Example: void (Object::*)() volatile noexcept          */
      inline static constexpr bool   IsConstVolatileNoexcept  = false; /* Example: void (Object::*)() const volatile noexcept    */
      inline static constexpr bool   IsL                      = false; /* Example: void (Object::*)() &                          */
      inline static constexpr bool   IsLConst                 = false; /* Example: void (Object::*)() const &                    */
      inline static constexpr bool   IsLVolatile              = false; /* Example: void (Object::*)() volatile &                 */
      inline static constexpr bool   IsLConstVolatile         = false; /* Example: void (Object::*)() const volatile &           */
      inline static constexpr bool   IsLNoexcept              = false; /* Example: void (Object::*)() & noexcept                 */
      inline static constexpr bool   IsLConstNoexcept         = false; /* Example: void (Object::*)() const & noexcept           */
      inline static constexpr bool   IsLVolatileNoexcept      = false; /* Example: void (Object::*)() volatile & noexcept        */
      inline static constexpr bool   IsLConstVolatileNoexcept = false; /* Example: void (Object::*)() const volatile & noexcept  */
      inline static constexpr bool   IsR                      = false; /* Example: void (Object::*)() &&                         */
      inline static constexpr bool   IsRConst                 = false; /* Example: void (Object::*)() const &&                   */
      inline static constexpr bool   IsRVolatile              = false; /* Example: void (Object::*)() volatile &&                */
      inline static constexpr bool   IsRConstVolatile         = false; /* Example: void (Object::*)() const volatile &&          */
      inline static constexpr bool   IsRNoexcept              = false; /* Example: void (Object::*)() && noexcept                */
      inline static constexpr bool   IsRConstNoexcept         = false; /* Example: void (Object::*)() const && noexcept          */
      inline static constexpr bool   IsRVolatileNoexcept      = false; /* Example: void (Object::*)() volatile && noexcept       */
      inline static constexpr bool   IsRConstVolatileNoexcept = false; /* Example: void (Object::*)() const volatile && noexcept */
  };

  template <typename ReturnType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (*)(ArgumentTypes...)> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      inline static constexpr bool IsPointerFunction = true;
  };

  template <typename ReturnType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (*)(ArgumentTypes...) noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      inline static constexpr bool IsPointerFunction = true;
      inline static constexpr bool IsNoexcept        = true;
  };

#define DEFINE_FUNCTION_TRAITS(...)                                                                                             \
 template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>                                                 \
 struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) __VA_ARGS__> : FunctionTraits<ReturnType(ArgumentTypes...)> \
 {                                                                                                                              \
   using Object = ObjectType;                                                                                                   \
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

  DEFINE_FUNCTION_TRAITS(&noexcept)
  DEFINE_FUNCTION_TRAITS(const& noexcept)
  DEFINE_FUNCTION_TRAITS(volatile& noexcept)
  DEFINE_FUNCTION_TRAITS(const volatile& noexcept)

  DEFINE_FUNCTION_TRAITS(&&)
  DEFINE_FUNCTION_TRAITS(const&&)
  DEFINE_FUNCTION_TRAITS(volatile&&)
  DEFINE_FUNCTION_TRAITS(const volatile&&)

  DEFINE_FUNCTION_TRAITS(&&noexcept)
  DEFINE_FUNCTION_TRAITS(const&& noexcept)
  DEFINE_FUNCTION_TRAITS(volatile&& noexcept)
  DEFINE_FUNCTION_TRAITS(const volatile&& noexcept)

#undef DEFINE_FUNCTION_TRAITS

  template <typename FunctionType>
  struct FunctionTraits<std::function<FunctionType>> : FunctionTraits<FunctionType>
  {
      using Object = std::function<FunctionType>;
  };

  template <typename CallableObject>
  struct FunctionTraits : FunctionTraits<decltype(&CallableObject::operator())>
  {
      using Object = CallableObject;
  };
} // namespace Cxx::Traits

#endif /* C2E87DC9_C92B_4119_AE64_4685318B7705 */
