cmake_minimum_required(VERSION 3.24)

project(
    CxxLibraries
    LANGUAGES C CXX
    VERSION 1.0.0.0
    DESCRIPTION "Librerías y patrones de diseños nativos escritos en C y/o C++"
    HOMEPAGE_URL "https://github.com/DenisRitchie/Cxx"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)

include(MessageLog)

MessageLog("${PROJECT_NAME} v${PROJECT_VERSION}: ${PROJECT_DESCRIPTION}")

if(MSVC)
    # https://learn.microsoft.com/en-us/cpp/build/reference/compiler-options?view=msvc-170
    # Warning level 4 and all warnings as errors
    add_compile_options(
        # /Wall	                 # Enable all warnings, including warnings that are disabled by default.
        /W4                      # Set output warning level.
        /WX                      # Treat warnings as errors.
        /await:strict            # Enable standard C++20 coroutine support with earlier language versions.
        /std:c++latest           # The latest draft C++ standard preview features.
        /Zc:__cplusplus          # Enable the __cplusplus macro to report the supported standard (off by default).
        /permissive-             # Set standard-conformance mode.
        /constexpr:backtrace10   # Show N constexpr evaluations in diagnostics (default: 10).
        /constexpr:depth512      # Recursion depth limit for constexpr evaluation (default: 512).
        /constexpr:steps100000   # Terminate constexpr evaluation after N steps (default: 100000).
    )

    add_compile_definitions(
        _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
        _SILENCE_CXX20_CISO646_REMOVED_WARNING
        _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
        _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
    )
else()
    # https://gcc.gnu.org/onlinedocs/gcc/Invoking-GCC.html
    # https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html
    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
    # Lots of warnings and all warnings as errors
    add_compile_options(
        # -Winvalid-constexpr          # This warning is enabled as a pedantic warning by default in C++20 and earlier. In C++23, -Winvalid-constexpr can be turned on, in which case it will be an ordinary warning.
        -Wall                          # This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros. This also enables some language-specific warnings described in C++ Dialect Options and Objective-C and Objective-C++ Dialect Options.
        -Wextra                        # This enables some extra warning flags that are not enabled by -Wall. (This option used to be called -W. The older name is still supported, but the newer name is more descriptive.)
        -Wpedantic                     # Issue all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++. For ISO C, follows the version of the ISO C standard specified by any -std option used.
        -Werror                        # Make all warnings into errors.
        -std=gnu++23                   # Determine the language standard. See Language Standards Supported by GCC, for details of these standard versions. This option is currently only supported when compiling C or C++.
        -fstrict-enums                 # Allow the compiler to optimize using the assumption that a value of enumerated type can only be one of the values of the enumeration (as defined in the C++ standard; basically, a value that can be represented in the minimum number of bits needed to represent all the enumerators). This assumption may not be valid if the program uses a cast to convert an arbitrary integer value to the enumerated type.
        -fpermissive                   # Downgrade some diagnostics about nonconformant code from errors to warnings. Thus, using -fpermissive allows some nonconforming code to compile.
        -fconstexpr-depth=512          # Set the maximum nested evaluation depth for C++11 constexpr functions to n. A limit is needed to detect endless recursion during constant expression evaluation. The minimum specified by the standard is 512.
        -fconstexpr-cache-depth=16     # Set the maximum level of nested evaluation depth for C++11 constexpr functions that will be cached to n. This is a heuristic that trades off compilation speed (when the cache avoids repeated calculations) against memory consumption (when the cache grows very large from highly recursive evaluations). The default is 8. Very few users are likely to want to adjust it, but if your code does heavy constexpr calculations you might want to experiment to find which value works best for you.
        -fconstexpr-loop-limit=262144  # Set the maximum number of iterations for a loop in C++14 constexpr functions to n. A limit is needed to detect infinite loops during constant expression evaluation. The default is 262144 (1<<18).
        -fconstexpr-ops-limit=33554432 # Set the maximum number of operations during a single constexpr evaluation. Even when number of iterations of a single loop is limited with the above limit, if there are several nested loops and each of them has many iterations but still smaller than the above limit, or if in a body of some loop or even outside of a loop too many expressions need to be evaluated, the resulting constexpr evaluation might take too long. The default is 33554432 (1<<25).
        -ftemplate-backtrace-limit=10  # Set the maximum number of template instantiation notes for a single warning or error to n. The default value is 10.
        -ftemplate-depth=1024          # Set the maximum instantiation depth for template classes to n. A limit on the template instantiation depth is needed to detect endless recursions during template class instantiation. ANSI/ISO C++ conforming programs must not rely on a maximum depth greater than 17 (changed to 1024 in C++11). The default value is 900, as the compiler can run out of stack space before hitting 1024 in some situations.
    )
endif()

add_library(${PROJECT_NAME} STATIC)
add_library(Cxx::Library::Core ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PUBLIC
        Includes/Cxx/Algorithms.hpp
        Includes/Cxx/CompilerFeature.hpp
        Includes/Cxx/Concepts.hpp
        Includes/Cxx/ContainerTraits.hpp
        Includes/Cxx/DetectionIdiom.hpp
        Includes/Cxx/ExtensionMethods.hpp
        Includes/Cxx/FunctionTraits.hpp
        Includes/Cxx/IteratorTraits.hpp
        Includes/Cxx/Optional.hpp
        Includes/Cxx/Platform.hpp
        Includes/Cxx/Reference.hpp
        Includes/Cxx/SemanticValue.hpp
        Includes/Cxx/SemiRegularBox.hpp
        Includes/Cxx/TypeTraits.hpp
        Includes/Cxx/Utility.hpp
        Includes/Cxx/Coroutines/Future.hpp
        Includes/Cxx/Coroutines/Generator.hpp
        Includes/Cxx/Exceptions/IOException.hpp
        Includes/Cxx/Exceptions/UnstableIteratorException.hpp
        Includes/Cxx/DesignPatterns/CuriouslyRecurringTemplatePattern.hpp
        Includes/Cxx/DesignPatterns/InputIterator.hpp
        Includes/Cxx/DesignPatterns/OutputIterator.hpp
        Includes/Cxx/DesignPatterns/ServiceLocator.hpp

    PUBLIC
        Sources/Cxx/Algorithms.cpp
        Sources/Cxx/Utility.cpp
        Sources/Cxx/DesignPatterns/ServiceLocator.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC c_std_17 cxx_std_23)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Includes)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Debug
    EXPORT CxxTargets
    LIBRARY DESTINATION Debug/Lib
    RUNTIME DESTINATION Debug/Lib
    ARCHIVE DESTINATION Debug/Lib
)

install(TARGETS ${PROJECT_NAME}
    CONFIGURATIONS Release
    LIBRARY DESTINATION Release/Lib
    RUNTIME DESTINATION Release/Lib
    ARCHIVE DESTINATION Release/Lib
)

install(
    FILES
        Includes/Cxx/CompilerFeature.hpp
        Includes/Cxx/ContainerTraits.hpp
        Includes/Cxx/DetectionIdiom.hpp
        Includes/Cxx/ExtensionMethods.hpp
        Includes/Cxx/Generator.hpp
        Includes/Cxx/IteratorTraits.hpp
        Includes/Cxx/Optional.hpp
        Includes/Cxx/Reference.hpp
        Includes/Cxx/SemanticValue.hpp
        Includes/Cxx/TypeTraits.hpp
    DESTINATION Include/Cxx
)

install(
    FILES
        Includes/Cxx/Exceptions/IOException.hpp
        Includes/Cxx/Exceptions/UnstableIteratorException.hpp
    DESTINATION Include/Cxx/Exceptions
)

install(
    FILES
        Includes/Cxx/DesignPatterns/OutputIteratorAdapter.hpp
        Includes/Cxx/DesignPatterns/ServiceLocator.hpp
    DESTINATION Include/Cxx/DesignPatterns
)

if(MSVC)
    enable_testing()
    include(CTest)
endif()

add_subdirectory(Tests)

# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html
include(CPack)
