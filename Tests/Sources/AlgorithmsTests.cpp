#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cxx/Algorithms.hpp"

#include <array>
#include <vector>

using Cxx::Algorithms::Join;
// using Cxx::Split;

using std::string;
using std::string_view;

namespace views = std::ranges::views;

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace NsAlgorithmsTests
{

  TEST(AlgorithmsTests, AlgorithmSplit)
  {
    // std::string_view      text   = ".1......2.....3....4...5..6.";
    // [[maybe_unused]] auto tokens = Split(".1......2.....3....4...5..6.", ".");

    // for ( size_t index = 0; const std::string_view token : tokens )
    // {
    //   std::cout << "[" << ++index << "]: " << token << std::endl;
    // }
  }

  TEST(AlgorithmsTests, RangesSplit)
  {
    // [[maybe_unused]] std::string_view text   = ".1......2.....3....4...5..6.";
    // [[maybe_unused]] auto             tokens = text | std::views::split(std::string_view{ "." });

    // for ( size_t index = 0; const auto token : tokens )
    // {
    //   std::string_view value(&*token.begin(), token.size());
    //   std::cout << "[" << ++index << "]: " << value << std::endl;
    // }
  }

  TEST(AlgorithmsTests, CompareRanges)
  {
    using namespace Cxx::Algorithms;

    EXPECT_FALSE(std::is_eq(RangeCompare("Hola", "HOLA", std::strong_order)));
    EXPECT_TRUE(std::is_neq(RangeCompare("Hola", "HOLA", std::strong_order)));
    EXPECT_FALSE(std::is_lt(RangeCompare("Hola", "HOLA", std::strong_order)));
    EXPECT_FALSE(std::is_lteq(RangeCompare("Hola", "HOLA", std::strong_order)));
    EXPECT_TRUE(std::is_gt(RangeCompare("Hola", "HOLA", std::strong_order)));
    EXPECT_TRUE(std::is_gteq(RangeCompare("Hola", "HOLA", std::strong_order)));

    EXPECT_TRUE(std::is_eq(RangeCompare("Hola", "HOLA", Cxx::CompareThreeWayOrderFallback, ::toupper, ::toupper)));
    EXPECT_FALSE(std::is_neq(RangeCompare("Hola", "HOLA", Cxx::CompareThreeWayOrderFallback, ::toupper, ::toupper)));
    EXPECT_FALSE(std::is_lt(RangeCompare("Hola", "HOLA", Cxx::CompareThreeWayOrderFallback, ::toupper, ::toupper)));
    EXPECT_TRUE(std::is_lteq(RangeCompare("Hola", "HOLA", Cxx::CompareThreeWayOrderFallback, ::toupper, ::toupper)));
    EXPECT_FALSE(std::is_gt(RangeCompare("Hola", "HOLA", Cxx::CompareThreeWayOrderFallback, ::toupper, ::toupper)));
    EXPECT_TRUE(std::is_gteq(RangeCompare("Hola", "HOLA", Cxx::CompareThreeWayOrderFallback, ::toupper, ::toupper)));

    {
      const auto& facet = std::use_facet<std::ctype<char>>(std::locale());
      EXPECT_TRUE(std::is_gteq(RangeCompare(
        "Hola",
        "HOLA",
        Cxx::CompareThreeWayOrderFallback, //
        [&](const char letter) { return facet.toupper(letter); },
        [&](const char letter) { return facet.toupper(letter); }
      )));
    }

    // EXPECT_TRUE(std::is_eq(StringCompare("Hola", "Hola")));
    // EXPECT_TRUE(std::is_eq(StringCompare("Hola", "HOLA", true)));
    // EXPECT_FALSE(std::is_eq(StringCompare("Hola", "HOLA")));
    // EXPECT_FALSE(std::is_eq(StringCompare("Hola", "Hi", true)));

    // {
    //   const char* text = "Hola";
    //   EXPECT_TRUE(std::is_eq(StringCompare(text, "HOLA", true)));
    // }

    // std::projected<const char*, decltype(&::toupper)>::value_type t;
  }

} // namespace NsAlgorithmsTests
