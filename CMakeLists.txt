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
    add_compile_options(/W4 /WX /await:strict /std:c++latest /Zc:__cplusplus)
else()
    # Lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(${PROJECT_NAME} STATIC)
add_library(Cxx::Library::Core ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PUBLIC
        Includes/Cxx/CompilerFeature.hpp
        Includes/Cxx/ContainerTraits.hpp
        Includes/Cxx/DetectionIdiom.hpp
        Includes/Cxx/ExtensionMethods.hpp
        Includes/Cxx/FunctionTraits.hpp
        Includes/Cxx/Generator.hpp
        Includes/Cxx/IteratorTraits.hpp
        Includes/Cxx/Optional.hpp
        Includes/Cxx/Platform.hpp
        Includes/Cxx/Reference.hpp
        Includes/Cxx/SemanticValue.hpp
        Includes/Cxx/SemiRegularBox.hpp
        Includes/Cxx/TypeTraits.hpp
        Includes/Cxx/Exceptions/IOException.hpp
        Includes/Cxx/Exceptions/UnstableIteratorException.hpp
        Includes/Cxx/DesignPatterns/OutputIteratorAdapter.hpp
        Includes/Cxx/DesignPatterns/ServiceLocator.hpp
        Includes/Cxx/String.hpp

    PUBLIC
        Sources/Cxx/DesignPatterns/ServiceLocator.cpp
        Sources/Cxx/String.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC c_std_17 cxx_std_23)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Includes)

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

add_subdirectory(Tests)

# https://cmake.org/cmake/help/book/mastering-cmake/chapter/Packaging%20With%20CPack.html
include(CPack)
