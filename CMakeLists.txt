cmake_minimum_required(VERSION 3.24)

project(
    NativeDesignPatterns
    LANGUAGES C CXX
    VERSION 1.0.0.0
    DESCRIPTION "Patrones de diseño nativos escritos en C y C++"
    HOMEPAGE_URL "https://github.com/DenisRitchie/NativeDesignPatterns"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)

include(MessageLog)

MessageLog("${PROJECT_NAME} v${PROJECT_VERSION}: ${PROJECT_DESCRIPTION}")

if (MSVC)
    # Warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX /await:strict)
else()
    # Lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PUBLIC
        Includes/Generator.hpp
        Includes/ExtensionMethods.hpp
        Includes/SemanticValue.hpp
        Includes/ServiceLocator.hpp
        Sources/ServiceLocator.cpp

    PRIVATE
        Includes/Details/Optional.hpp
        Includes/Details/Reference.hpp
        Includes/Details/TypeTraits.hpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/Includes)

add_subdirectory(Tests)
