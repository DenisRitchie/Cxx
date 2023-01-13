#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <any>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <algorithm>
#include <utility>
#include <locale>
#include <ranges>

#include "Cxx/DesignPatterns/InputIterator.hpp"
#include "Cxx/DesignPatterns/OutputIterator.hpp"

using ::testing::ContainerEq;

using Cxx::DesignPatterns::MakeOutputIterator;
using Cxx::DesignPatterns::OutputIteratorAdapter;

TEST(OutputIteratorAdapterTests, OutputIteratorAdapterWithObjectFunction)
{
  std::string_view  text = "Este es un texto de prueba";
  std::vector<char> result;

  struct ObjectFunction
  {
      ObjectFunction(std::vector<char>& result) noexcept
        : m_Result{ result }
      {
        result.clear();
      }

      void operator()(const char value)
      {
        m_Result.push_back(value);
      }

      std::vector<char>& m_Result;
  };

  std::copy(text.crbegin(), text.crend(), MakeOutputIterator(ObjectFunction{ result }));
  EXPECT_TRUE(std::equal(text.cbegin(), text.cend(), result.crbegin()));

  std::copy(text.cbegin(), text.cend(), MakeOutputIterator<ObjectFunction>(result));
  EXPECT_TRUE(std::equal(text.crbegin(), text.crend(), result.crbegin()));
}

TEST(OutputIteratorAdapterTests, OutputIteratorAdapterWithLambda)
{
  std::string_view  text = "TextoDePrueba";
  std::vector<char> result;

  std::ranges::copy(text, MakeOutputIterator([&](const char value) { result.emplace_back(::toupper(value)); }));
  EXPECT_TRUE(std::ranges::all_of(result, ::isupper));

  result.clear();

  std::ranges::copy(text, MakeOutputIterator([&](const char value) { result.emplace_back(::tolower(value)); }));
  EXPECT_TRUE(std::ranges::all_of(result, ::islower));
}

TEST(OutputIteratorAdapterTests, OutputIteratorAdapterWithStdFunction)
{
  std::string_view text = "Este es un texto de prueba";
  std::list<char>  result;

  std::copy(
    text.crbegin(),
    text.crend(),
    MakeOutputIterator // clang-format off
    (
      std::function<void(const char)>
      {
        [&result](const char value)
        {
          result.emplace_back(value);
        }
      }
    ) // clang-format on
  );

  EXPECT_TRUE(std::equal(text.cbegin(), text.cend(), result.crbegin()));

  result.clear();

  std::copy(
    text.cbegin(),
    text.cend(),
    MakeOutputIterator<std::function<void(const char)>>( //
      [&result](const char value) { result.emplace_front(value); }
    )
  );

  EXPECT_TRUE(std::equal(text.crbegin(), text.crend(), result.cbegin()));
}
