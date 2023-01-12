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
#include "Cxx/DesignPatterns/OutputIteratorAdapter.hpp"

using Cxx::DesignPatterns::MakeOutputIterator;
using Cxx::DesignPatterns::OutputIteratorAdapter;

TEST(OutputIteratorAdapterTests, OutputIteratorAdapterWithObjectFunction)
{
  [[maybe_unused]] const auto&       ctype_facet = std::use_facet<std::ctype_byname<char>>(std::locale());
  [[maybe_unused]] std::string_view  text        = "Este es un texto de prueba";
  [[maybe_unused]] std::vector<char> result;

  struct ObjectFunction
  {
      ObjectFunction(std::vector<char>& result) noexcept
        : m_Result{ result }
      {
      }

      void operator()(const char value)
      {
        m_Result.push_back(value);
      }

      std::vector<char>& m_Result;
  };

  std::copy(text.crbegin(), text.crend(), MakeOutputIterator<ObjectFunction>(result));
  EXPECT_TRUE(std::equal(text.cbegin(), text.cend(), result.crbegin()));
}
