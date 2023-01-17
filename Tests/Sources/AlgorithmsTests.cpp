#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cxx/Algorithms.hpp"

#include <array>
#include <vector>

using Cxx::Join;
using Cxx::Split;

using std::string;
using std::string_view;

namespace views = std::ranges::views;

using namespace std::string_literals;
using namespace std::string_view_literals;

TEST(AlgorithmsTests, AlgorithmSplit)
{
  std::string_view      text   = ".1......2.....3....4...5..6.";
  [[maybe_unused]] auto tokens = Split(".1......2.....3....4...5..6.", ".");

  for ( size_t index = 0; const std::string_view token : tokens )
  {
    std::cout << "[" << ++index << "]: " << token << std::endl;
  }
}
