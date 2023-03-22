#ifndef D7E252FA_D919_494D_8B42_D1B31F3710C0
#define D7E252FA_D919_494D_8B42_D1B31F3710C0

#ifndef HAS_CXX17
# if __cplusplus > 201402L
#  define HAS_CXX17 1
# else
#  define HAS_CXX17 0
# endif
#endif // HAS_CXX17

#ifndef HAS_CXX20
# if HAS_CXX17 && __cplusplus > 201703L
#  define HAS_CXX20 1
# else
#  define HAS_CXX20 0
# endif
#endif // HAS_CXX20

#ifndef HAS_CXX23
# if HAS_CXX20 && __cplusplus > 202002L
#  define HAS_CXX23 1
# else
#  define HAS_CXX23 0
# endif
#endif // HAS_CXX23

#if HAS_CXX20 && !HAS_CXX17
# error HAS_CXX20 must imply HAS_CXX17.
#endif

#if HAS_CXX23 && !HAS_CXX20
# error HAS_CXX23 must imply HAS_CXX20.
#endif

// C++23: https://eel.is/c++draft/tab:cpp.predefined.ft
#if HAS_CXX23
# if defined(_MSC_VER)
#  ifndef __cpp_explicit_this_parameter
#   define __cpp_explicit_this_parameter 202110L
#  endif
#  ifndef __cpp_multidimensional_subscript
#   define __cpp_multidimensional_subscript 202211L
#  endif
# endif
#endif // HAS_CXX23

// https://www.fluentcpp.com/2019/08/30/how-to-disable-a-warning-in-cpp/
// https://learn.microsoft.com/en-us/cpp/preprocessor/pragma-directives-and-the-pragma-keyword?view=msvc-170
// https://gcc.gnu.org/onlinedocs/cpp/Pragmas.html

// clang-format off

#if defined(_MSC_VER)
  #define DISABLE_WARNING_PUSH                          __pragma(warning(push))
  #define DISABLE_WARNING_POP                           __pragma(warning(pop))
  #define DISABLE_WARNING(WarningNumber)                __pragma(warning(disable: WarningNumber))

  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER DISABLE_WARNING(4100)
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION         DISABLE_WARNING(4505)
  #define DISABLE_WARNING_RESERVED_USER_DEFINED_LITERAL DISABLE_WARNING(4455)
  // Other warnings you want to deactivate...
#elif defined(__GNUC__) || defined(__clang__)
  #define DO_PRAGMA(X)                                  _Pragma(#X)
  #define DISABLE_WARNING_PUSH                          DO_PRAGMA(GCC diagnostic push)
  #define DISABLE_WARNING_POP                           DO_PRAGMA(GCC diagnostic pop)
  #define DISABLE_WARNING(WarningName)                  DO_PRAGMA(GCC diagnostic ignored #WarningName)

  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER DISABLE_WARNING(-Wunused-parameter)
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION         DISABLE_WARNING(-Wunused-function)
  #define DISABLE_WARNING_RESERVED_USER_DEFINED_LITERAL DISABLE_WARNING(-Wliteral-suffix)
  // Other warnings you want to deactivate...
#else
  #define DISABLE_WARNING_PUSH
  #define DISABLE_WARNING_POP
  #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
  #define DISABLE_WARNING_UNREFERENCED_FUNCTION
  #define DISABLE_WARNING_RESERVED_USER_DEFINED_LITERAL
  // Other warnings you want to deactivate...
#endif

// clang-format on

#endif /* D7E252FA_D919_494D_8B42_D1B31F3710C0 */
