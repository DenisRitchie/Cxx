cmake_minimum_required(VERSION 3.24)

project(
    CxxLibrariesTests
    LANGUAGES C CXX
    VERSION 1.0.0.0
    DESCRIPTION "Test: ${CMAKE_PROJECT_DESCRIPTION}"
    HOMEPAGE_URL "https://github.com/DenisRitchie/Cxx"
)

MessageLog("${PROJECT_NAME} v${PROJECT_VERSION}: ${PROJECT_DESCRIPTION}")

file(GLOB_RECURSE cpp_source_files
    LIST_DIRECTORIES false
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "*.cpp" "*.hpp" "*.c" "*.h" "*.cxx" "*.cc")

MessageLogArray("${cpp_source_files}")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 23)

# https://www.foonathan.net/2022/06/cmake-fetchcontent/
# https://cmake.org/cmake/help/latest/module/FetchContent.html

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_PROJECT_NAME CxxLibrariesTests)

add_executable(${TEST_PROJECT_NAME} ${cpp_source_files})

target_link_libraries(${TEST_PROJECT_NAME}
    PUBLIC
        GTest::gtest_main
        GTest::gmock_main
        Cxx::Library::Core
)

target_include_directories(${TEST_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(${TEST_PROJECT_NAME} PUBLIC c_std_17 cxx_std_23)

include(GoogleTest)
gtest_discover_tests(${TEST_PROJECT_NAME} TEST_PREFIX Cxx::Libraries)

if(MSVC)
    add_test(NAME ${TEST_PROJECT_NAME} COMMAND ${TEST_PROJECT_NAME})
endif()
