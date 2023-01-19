#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cxx/Algorithms.hpp"

#include <array>
#include <vector>

using Cxx::Join;
// using Cxx::Split;

using std::string;
using std::string_view;

namespace views = std::ranges::views;

using namespace std::string_literals;
using namespace std::string_view_literals;

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
  EXPECT_FALSE(std::is_eq(Cxx::RangeCompare("Hola", "HOLA")));
  EXPECT_TRUE(std::is_neq(Cxx::RangeCompare("Hola", "HOLA")));
  EXPECT_FALSE(std::is_lt(Cxx::RangeCompare("Hola", "HOLA")));
  EXPECT_FALSE(std::is_lteq(Cxx::RangeCompare("Hola", "HOLA")));
  EXPECT_TRUE(std::is_gt(Cxx::RangeCompare("Hola", "HOLA")));
  EXPECT_TRUE(std::is_gteq(Cxx::RangeCompare("Hola", "HOLA")));

  EXPECT_TRUE(std::is_eq(Cxx::RangeCompare("Hola", "HOLA", std::ranges::equal_to{}, ::toupper, ::toupper)));
  EXPECT_FALSE(std::is_neq(Cxx::RangeCompare("Hola", "HOLA", std::ranges::equal_to{}, ::toupper, ::toupper)));
  EXPECT_FALSE(std::is_lt(Cxx::RangeCompare("Hola", "HOLA", std::ranges::equal_to{}, ::toupper, ::toupper)));
  EXPECT_TRUE(std::is_lteq(Cxx::RangeCompare("Hola", "HOLA", std::ranges::equal_to{}, ::toupper, ::toupper)));
  EXPECT_FALSE(std::is_gt(Cxx::RangeCompare("Hola", "HOLA", std::ranges::equal_to{}, ::toupper, ::toupper)));
  EXPECT_TRUE(std::is_gteq(Cxx::RangeCompare("Hola", "HOLA", std::ranges::equal_to{}, ::toupper, ::toupper)));
}
