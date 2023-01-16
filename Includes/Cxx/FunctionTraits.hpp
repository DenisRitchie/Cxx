// https://en.cppreference.com/w/cpp/language/parameter_pack
// https://en.cppreference.com/w/cpp/language/sizeof...
// https://en.cppreference.com/w/cpp/language/fold
// https://en.cppreference.com/w/cpp/language/function
// https://en.cppreference.com/w/cpp/types/is_function

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

  /***************************************************************************************************************************************/

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

  /***************************************************************************************************************************************/

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...)> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsLConst         = true;
      inline static constexpr bool IsRConst         = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) volatile> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsVolatile       = true;
      inline static constexpr bool IsLVolatile      = true;
      inline static constexpr bool IsRVolatile      = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const volatile> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsVolatile       = true;
      inline static constexpr bool IsConstVolatile  = true;
      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsLConst         = true;
      inline static constexpr bool IsLVolatile      = true;
      inline static constexpr bool IsLConstVolatile = true;
      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsRConst         = true;
      inline static constexpr bool IsRVolatile      = true;
      inline static constexpr bool IsRConstVolatile = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  /***************************************************************************************************************************************/

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsNoexcept       = true;
      inline static constexpr bool IsLNoexcept      = true;
      inline static constexpr bool IsRNoexcept      = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsNoexcept       = true;
      inline static constexpr bool IsConstNoexcept  = true;
      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsLConst         = true;
      inline static constexpr bool IsLNoexcept      = true;
      inline static constexpr bool IsLConstNoexcept = true;
      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsRConst         = true;
      inline static constexpr bool IsRNoexcept      = true;
      inline static constexpr bool IsRConstNoexcept = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) volatile noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsVolatile          = true;
      inline static constexpr bool IsNoexcept          = true;
      inline static constexpr bool IsVolatileNoexcept  = true;
      inline static constexpr bool IsL                 = true;
      inline static constexpr bool IsLVolatile         = true;
      inline static constexpr bool IsLNoexcept         = true;
      inline static constexpr bool IsLVolatileNoexcept = true;
      inline static constexpr bool IsR                 = true;
      inline static constexpr bool IsRVolatile         = true;
      inline static constexpr bool IsRNoexcept         = true;
      inline static constexpr bool IsRVolatileNoexcept = true;
      inline static constexpr bool IsMemberFunction    = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const volatile noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst                  = true; // #1
      inline static constexpr bool IsVolatile               = true; // #2
      inline static constexpr bool IsConstVolatile          = true; // #3
      inline static constexpr bool IsNoexcept               = true; // #4
      inline static constexpr bool IsConstNoexcept          = true; // #5
      inline static constexpr bool IsVolatileNoexcept       = true; // #6
      inline static constexpr bool IsConstVolatileNoexcept  = true; // #7
      inline static constexpr bool IsL                      = true; // #8
      inline static constexpr bool IsLConst                 = true; // #9
      inline static constexpr bool IsLVolatile              = true; // #10
      inline static constexpr bool IsLConstVolatile         = true; // #11
      inline static constexpr bool IsLNoexcept              = true; // #12
      inline static constexpr bool IsLConstNoexcept         = true; // #13
      inline static constexpr bool IsLVolatileNoexcept      = true; // #14
      inline static constexpr bool IsLConstVolatileNoexcept = true; // #15
      inline static constexpr bool IsR                      = true; // #16
      inline static constexpr bool IsRConst                 = true; // #17
      inline static constexpr bool IsRVolatile              = true; // #18
      inline static constexpr bool IsRConstVolatile         = true; // #19
      inline static constexpr bool IsRNoexcept              = true; // #20
      inline static constexpr bool IsRConstNoexcept         = true; // #21
      inline static constexpr bool IsRVolatileNoexcept      = true; // #22
      inline static constexpr bool IsRConstVolatileNoexcept = true; // #23
      inline static constexpr bool IsMemberFunction         = true; // #24
  };

  /***************************************************************************************************************************************/

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...)&> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const&> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsLConst         = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) volatile&> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsVolatile       = true;
      inline static constexpr bool IsLVolatile      = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const volatile&> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsVolatile       = true;
      inline static constexpr bool IsConstVolatile  = true;
      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsLConst         = true;
      inline static constexpr bool IsLVolatile      = true;
      inline static constexpr bool IsLConstVolatile = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  /***************************************************************************************************************************************/

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) & noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsNoexcept       = true;
      inline static constexpr bool IsLNoexcept      = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const & noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsNoexcept       = true;
      inline static constexpr bool IsConstNoexcept  = true;
      inline static constexpr bool IsL              = true;
      inline static constexpr bool IsLConst         = true;
      inline static constexpr bool IsLNoexcept      = true;
      inline static constexpr bool IsLConstNoexcept = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) volatile & noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsVolatile          = true;
      inline static constexpr bool IsNoexcept          = true;
      inline static constexpr bool IsVolatileNoexcept  = true;
      inline static constexpr bool IsL                 = true;
      inline static constexpr bool IsLVolatile         = true;
      inline static constexpr bool IsLNoexcept         = true;
      inline static constexpr bool IsLVolatileNoexcept = true;
      inline static constexpr bool IsMemberFunction    = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const volatile & noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst                  = true; // #1
      inline static constexpr bool IsVolatile               = true; // #2
      inline static constexpr bool IsConstVolatile          = true; // #3
      inline static constexpr bool IsNoexcept               = true; // #4
      inline static constexpr bool IsConstNoexcept          = true; // #5
      inline static constexpr bool IsVolatileNoexcept       = true; // #6
      inline static constexpr bool IsConstVolatileNoexcept  = true; // #7
      inline static constexpr bool IsL                      = true; // #8
      inline static constexpr bool IsLConst                 = true; // #9
      inline static constexpr bool IsLVolatile              = true; // #10
      inline static constexpr bool IsLConstVolatile         = true; // #11
      inline static constexpr bool IsLNoexcept              = true; // #12
      inline static constexpr bool IsLConstNoexcept         = true; // #13
      inline static constexpr bool IsLVolatileNoexcept      = true; // #14
      inline static constexpr bool IsLConstVolatileNoexcept = true; // #15
      inline static constexpr bool IsMemberFunction         = true; // #16
  };

  /***************************************************************************************************************************************/

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) &&> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const&&> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsRConst         = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) volatile&&> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsVolatile       = true;
      inline static constexpr bool IsRVolatile      = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const volatile&&> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsVolatile       = true;
      inline static constexpr bool IsConstVolatile  = true;
      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsRConst         = true;
      inline static constexpr bool IsRVolatile      = true;
      inline static constexpr bool IsRConstVolatile = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  /***************************************************************************************************************************************/

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) && noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsNoexcept       = true;
      inline static constexpr bool IsRNoexcept      = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const && noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst          = true;
      inline static constexpr bool IsNoexcept       = true;
      inline static constexpr bool IsConstNoexcept  = true;
      inline static constexpr bool IsR              = true;
      inline static constexpr bool IsRConst         = true;
      inline static constexpr bool IsRNoexcept      = true;
      inline static constexpr bool IsRConstNoexcept = true;
      inline static constexpr bool IsMemberFunction = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) volatile && noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsVolatile          = true;
      inline static constexpr bool IsNoexcept          = true;
      inline static constexpr bool IsVolatileNoexcept  = true;
      inline static constexpr bool IsR                 = true;
      inline static constexpr bool IsRVolatile         = true;
      inline static constexpr bool IsRNoexcept         = true;
      inline static constexpr bool IsRVolatileNoexcept = true;
      inline static constexpr bool IsMemberFunction    = true;
  };

  template <typename ReturnType, typename ObjectType, typename... ArgumentTypes>
  struct FunctionTraits<ReturnType (ObjectType::*)(ArgumentTypes...) const volatile && noexcept> : FunctionTraits<ReturnType(ArgumentTypes...)>
  {
      using Object = ObjectType;

      inline static constexpr bool IsConst                  = true; // #1
      inline static constexpr bool IsVolatile               = true; // #2
      inline static constexpr bool IsConstVolatile          = true; // #3
      inline static constexpr bool IsNoexcept               = true; // #4
      inline static constexpr bool IsConstNoexcept          = true; // #5
      inline static constexpr bool IsVolatileNoexcept       = true; // #6
      inline static constexpr bool IsConstVolatileNoexcept  = true; // #7
      inline static constexpr bool IsR                      = true; // #8
      inline static constexpr bool IsRConst                 = true; // #9
      inline static constexpr bool IsRVolatile              = true; // #10
      inline static constexpr bool IsRConstVolatile         = true; // #11
      inline static constexpr bool IsRNoexcept              = true; // #12
      inline static constexpr bool IsRConstNoexcept         = true; // #13
      inline static constexpr bool IsRVolatileNoexcept      = true; // #14
      inline static constexpr bool IsRConstVolatileNoexcept = true; // #15
      inline static constexpr bool IsMemberFunction         = true; // #16
  };

  /***************************************************************************************************************************************/

  template <typename FunctionType>
  struct FunctionTraits<std::function<FunctionType>> : FunctionTraits<FunctionType>
  {
      using Object = std::function<FunctionType>;

      inline static constexpr bool IsStdFunction    = true;
      inline static constexpr bool IsObjectFunction = true;
  };

  template <typename CallableObject>
  struct FunctionTraits : FunctionTraits<decltype(&CallableObject::operator())>
  {
      using Object = CallableObject;

      inline static constexpr bool IsObjectFunction = true;
  };
} // namespace Cxx::Traits

#endif /* C2E87DC9_C92B_4119_AE64_4685318B7705 */
