cmake_minimum_required(VERSION 3.24)

project(
    NativeDesignPatternTests
    LANGUAGES C CXX
    VERSION 1.0.0.0
    DESCRIPTION "Test: ${CMAKE_PROJECT_DESCRIPTION}"
    HOMEPAGE_URL "https://github.com/DenisRitchie/NativeDesignPatterns"
)

MessageLog("${PROJECT_NAME} v${PROJECT_VERSION}: ${PROJECT_DESCRIPTION}")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_PROJECT_NAME NativeDesignPatternTests)

add_executable(${TEST_PROJECT_NAME} MainTests.cpp ServiceLocatorTests.cpp)

target_link_libraries(${TEST_PROJECT_NAME}
    PUBLIC
        GTest::gtest_main
        GTest::gmock_main
        NativeDesignPatterns
)

target_include_directories(${TEST_PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Includes
)

target_compile_features(${TEST_PROJECT_NAME} PUBLIC cxx_std_23)

include(GoogleTest)
gtest_discover_tests(${TEST_PROJECT_NAME} TEST_PREFIX NativeDesignPatterns)
