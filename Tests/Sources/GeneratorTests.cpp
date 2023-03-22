#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <ranges>
#include <vector>
#include <tuple>
#include <string>
#include <string_view>

#include "Cxx/ContainerTraits.hpp"
#include "Cxx/Coroutines/Future.hpp"
#include "Cxx/Coroutines/Generator.hpp"

using Cxx::Coroutines::Generator;
using testing::ContainerEq;

TEST(GeneratorTests, GeneratorContainerTraits)
{
  EXPECT_TRUE((Cxx::Concepts::InputContainer<Cxx::Coroutines::Generator<int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::InputIterator<Cxx::Coroutines::Generator<int32_t>::iterator>));
  EXPECT_TRUE((Cxx::Concepts::Cpp20InputIterator<Cxx::Coroutines::Generator<int32_t>::iterator>));
}

TEST(GeneratorTests, GeneratorOfPrimitiveValues)
{
  auto Range = [](const size_t first, const size_t last) -> Generator<size_t>
  {
    for ( size_t index = first; index <= last; ++index )
    {
      co_yield index;
    }
  };

  auto iota_view = std::ranges::views::iota(1, 101);
  EXPECT_EQ(iota_view.size(), 100);
  EXPECT_TRUE(std::ranges::equal(iota_view, Range(1, 100)));
}

TEST(GeneratorTests, GeneratorQueries)
{
  std::vector<std::string> values = { "Denis", "Javier", "West", "Johan", "Jeremy", "Kenneth", "Juan", "David" };

  auto names_by_word_size = [&values](const size_t word_size) -> Generator<std::tuple<int32_t, std::string_view>>
  {
    for ( int32_t index = -1; const auto& value : values )
    {
      if ( ++index; value.length() == word_size )
        co_yield { index, value };
    }
  };

  std::ranges::for_each(
    names_by_word_size(5),
    [&values](const auto& data)
    {
      const auto& [index, name] = data;
      EXPECT_EQ(values[index], name);
    }
  );
}
