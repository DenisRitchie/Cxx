// References:
//    https://google.github.io/googletest/gmock_cheat_sheet.html
//    https://google.github.io/googletest/gmock_cook_book.html
//    https://google.github.io/googletest/gmock_for_dummies.html

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cxx/Algorithms.hpp"

#include <array>
#include <vector>
#include <span>
#include <spanstream>

using ::testing::AtLeast;
using ::testing::Eq;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::Le;
using ::testing::Lt;
using ::testing::Ne;

using Cxx::Algorithms::Join;
using Cxx::Algorithms::Split;

using std::array;
using std::span;
using std::string;
using std::string_view;
using std::vector;

using namespace std::string_literals;
using namespace std::string_view_literals;

using namespace Cxx::StringLiterals;

namespace views = std::ranges::views;

TEST(AlgorithmsTests, AlgorithmSplit)
{
  using enum Cxx::Algorithms::StringSplitOptions;

  auto&& numbers = Split<RemoveEmptyEntries>(".1......2.....3....4...5..6.", ".") | std::ranges::to<vector>();
  auto&& tokens  = Split<None>(".1......2.....3....4...5..6.", ".") | std::ranges::to<vector>();

  EXPECT_EQ(tokens.size(), 23);
  EXPECT_EQ(numbers.size(), 6);

  EXPECT_EQ(numbers[0], "1");
  EXPECT_EQ(numbers[1], "2");
  EXPECT_EQ(numbers[2], "3");
  EXPECT_EQ(numbers[3], "4");
  EXPECT_EQ(numbers[4], "5");
  EXPECT_EQ(numbers[5], "6");

  EXPECT_EQ(tokens[1], "1");
  EXPECT_EQ(tokens[7], "2");
  EXPECT_EQ(tokens[12], "3");
  EXPECT_EQ(tokens[16], "4");
  EXPECT_EQ(tokens[19], "5");
  EXPECT_EQ(tokens[21], "6");
}

TEST(AlgorithmsTests, CompareStrings)
{
  using namespace Cxx::Algorithms;
  using namespace Cxx::StringLiterals;

  const auto& facet = std::use_facet<std::ctype<char>>(std::locale());

  // clang-format off
  EXPECT_FALSE(std::is_eq  (RangeCompare("abcde", "ABCDE")));
  EXPECT_TRUE (std::is_neq (RangeCompare("abcde", "ABCDE")));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE")));
  EXPECT_FALSE(std::is_lteq(RangeCompare("abcde", "ABCDE")));
  EXPECT_TRUE (std::is_gt  (RangeCompare("abcde", "ABCDE")));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE")));
  // clang-format on

  // clang-format off
  EXPECT_FALSE(std::is_eq  (RangeCompare("abcde", "ABCDE", Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_neq (RangeCompare("abcde", "ABCDE", Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_lteq(RangeCompare("abcde", "ABCDE", Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_gt  (RangeCompare("abcde", "ABCDE", Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", Cxx::CompareThreeWayOrderFallback)));
  // clang-format on

  // clang-format off
  EXPECT_TRUE (std::is_eq  (RangeCompare("abcde", "ABCDE", toupper)));
  EXPECT_FALSE(std::is_neq (RangeCompare("abcde", "ABCDE", toupper)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", toupper)));
  EXPECT_TRUE (std::is_lteq(RangeCompare("abcde", "ABCDE", toupper)));
  EXPECT_FALSE(std::is_gt  (RangeCompare("abcde", "ABCDE", toupper)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", toupper)));
  // clang-format on

  // clang-format off
  EXPECT_TRUE (std::is_eq  (RangeCompare("abcde", "ABCDE", toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_neq (RangeCompare("abcde", "ABCDE", toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_lteq(RangeCompare("abcde", "ABCDE", toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_gt  (RangeCompare("abcde", "ABCDE", toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", toupper, Cxx::CompareThreeWayOrderFallback)));
  // clang-format on

  // clang-format off
  EXPECT_TRUE (std::is_eq  (RangeCompare("abcde", "ABCDE", toupper, toupper)));
  EXPECT_FALSE(std::is_neq (RangeCompare("abcde", "ABCDE", toupper, toupper)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", toupper, toupper)));
  EXPECT_TRUE (std::is_lteq(RangeCompare("abcde", "ABCDE", toupper, toupper)));
  EXPECT_FALSE(std::is_gt  (RangeCompare("abcde", "ABCDE", toupper, toupper)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", toupper, toupper)));
  // clang-format on

  // clang-format off
  EXPECT_TRUE (std::is_eq  (RangeCompare("abcde", "ABCDE", toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_neq (RangeCompare("abcde", "ABCDE", toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_lteq(RangeCompare("abcde", "ABCDE", toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_gt  (RangeCompare("abcde", "ABCDE", toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  // clang-format on

  // clang-format off
  EXPECT_FALSE(std::is_eq  (RangeCompare("abcde", "ABCDE", 5)));
  EXPECT_TRUE (std::is_neq (RangeCompare("abcde", "ABCDE", 5)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", 5)));
  EXPECT_FALSE(std::is_lteq(RangeCompare("abcde", "ABCDE", 5)));
  EXPECT_TRUE (std::is_gt  (RangeCompare("abcde", "ABCDE", 5)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", 5)));
  // clang-format on

  // clang-format off
  EXPECT_FALSE(std::is_eq  (RangeCompare("abcde", "ABCDE", 5, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_neq (RangeCompare("abcde", "ABCDE", 5, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", 5, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_lteq(RangeCompare("abcde", "ABCDE", 5, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_gt  (RangeCompare("abcde", "ABCDE", 5, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", 5, Cxx::CompareThreeWayOrderFallback)));
  // clang-format on

  // clang-format off
  EXPECT_TRUE (std::is_eq  (RangeCompare("abcde", "ABCDE", 5, toupper)));
  EXPECT_FALSE(std::is_neq (RangeCompare("abcde", "ABCDE", 5, toupper)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", 5, toupper)));
  EXPECT_TRUE (std::is_lteq(RangeCompare("abcde", "ABCDE", 5, toupper)));
  EXPECT_FALSE(std::is_gt  (RangeCompare("abcde", "ABCDE", 5, toupper)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", 5, toupper)));
  // clang-format on

  // clang-format off
  EXPECT_TRUE (std::is_eq  (RangeCompare("abcde", "ABCDE", 5, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_neq (RangeCompare("abcde", "ABCDE", 5, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", 5, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_lteq(RangeCompare("abcde", "ABCDE", 5, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_gt  (RangeCompare("abcde", "ABCDE", 5, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", 5, toupper, Cxx::CompareThreeWayOrderFallback)));
  // clang-format on

  // clang-format off
  EXPECT_TRUE (std::is_eq  (RangeCompare("abcde", "ABCDE", 5, toupper, toupper)));
  EXPECT_FALSE(std::is_neq (RangeCompare("abcde", "ABCDE", 5, toupper, toupper)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", 5, toupper, toupper)));
  EXPECT_TRUE (std::is_lteq(RangeCompare("abcde", "ABCDE", 5, toupper, toupper)));
  EXPECT_FALSE(std::is_gt  (RangeCompare("abcde", "ABCDE", 5, toupper, toupper)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", 5, toupper, toupper)));
  // clang-format on

  // clang-format off
  EXPECT_TRUE (std::is_eq  (RangeCompare("abcde", "ABCDE", 5, toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_neq (RangeCompare("abcde", "ABCDE", 5, toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_lt  (RangeCompare("abcde", "ABCDE", 5, toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_lteq(RangeCompare("abcde", "ABCDE", 5, toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_FALSE(std::is_gt  (RangeCompare("abcde", "ABCDE", 5, toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  EXPECT_TRUE (std::is_gteq(RangeCompare("abcde", "ABCDE", 5, toupper, toupper, Cxx::CompareThreeWayOrderFallback)));
  // clang-format on

  EXPECT_TRUE(std::is_eq(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_FALSE(std::is_neq(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_FALSE(std::is_lt(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_TRUE(std::is_lteq(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_FALSE(std::is_gt(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_TRUE(std::is_gteq(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));

  EXPECT_TRUE(std::is_eq(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_FALSE(std::is_neq(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_FALSE(std::is_lt(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_TRUE(std::is_lteq(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_FALSE(std::is_gt(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
  EXPECT_TRUE(std::is_gteq(RangeCompare(
    "abcde", "ABCDE", [&](const char letter) { return facet.toupper(letter); }, [&](const char letter) { return facet.toupper(letter); }, Cxx::CompareThreeWayOrderFallback
  )));
}
