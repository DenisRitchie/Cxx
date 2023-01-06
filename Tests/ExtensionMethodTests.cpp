#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cxx/Generator.hpp"
#include "Cxx/ExtensionMethods.hpp"
#include "Cxx/TypeTraits.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <type_traits>
#include <iterator>
#include <algorithm>
#include <functional>
#include <optional>
#include <memory>
#include <sstream>

#pragma warning(push)
#pragma warning(disable : 4455)
using std::literals::operator""s;
using std::literals::operator""sv;
#pragma warning(pop)

using namespace Cxx;
using namespace Cxx::LINQ::MethodSyntax;

auto Append(auto&& Value) noexcept
{
  return [&]<typename CharType, typename TraitType, typename AllocatorType>(const std::basic_string<CharType, TraitType, AllocatorType> String)
  {
    return const_cast<std::basic_string<CharType, TraitType, AllocatorType>&>(String).append(Value);
  };
}

auto ShowLog() noexcept
{
  return [&](const std::string& Value) -> void
  {
    printf_s("Log: (String) %s\n", Value.c_str());
  };
}

auto Print()
{
  return []<typename Range>(Range&& Values)
  {
    std::cout << "\nType: " << typeid(Range).name() << std::endl;

    for ( int32_t Index = 0; const auto& Value : Values )
    {
      std::cout << "[" << ++Index << "]: " << Value << std::endl;
    }

    std::cout << std::endl;
  };
}

TEST(ExtensionMethodTest, PropertyExtension)
{
  std::string Value = "Denis West";

  EXPECT_EQ(Value->*Size, 10);
  EXPECT_EQ(Value->*Count, 10);
  EXPECT_EQ(Value->*Length, 10);
  EXPECT_EQ(Value->*LowerCase, "denis west");
  EXPECT_EQ(Value->*UpperCase, "DENIS WEST");
  EXPECT_EQ(Value->*UpperCase->*Length, 10);
}

TEST(ExtensionMethodTest, BooleanStringPropertyExtension)
{
  std::ostringstream StreamTest;
  StreamTest.setstate(StreamTest.badbit);

  const char8_t* Text1 = u8"Test";
  const char8_t* Text2 = u8"";

  EXPECT_EQ(Text1->*BooleanString, "True");
  EXPECT_EQ(Text2->*BooleanString, "False");
  EXPECT_EQ(true->*BooleanString, "True");
  EXPECT_EQ(false->*BooleanString, "False");
  EXPECT_EQ(""->*BooleanString, "False");
  EXPECT_EQ(""s->*BooleanString, "False");
  EXPECT_EQ(""sv->*BooleanString, "False");
  EXPECT_EQ("0"->*BooleanString, "True");
  EXPECT_EQ("0"s->*BooleanString, "True");
  EXPECT_EQ("0"sv->*BooleanString, "True");
  EXPECT_EQ(0->*BooleanString, "False");
  EXPECT_EQ(1->*BooleanString, "True");
  EXPECT_EQ(std::cout->*BooleanString, "True");
  EXPECT_EQ(StreamTest->*BooleanString, "False");
  EXPECT_EQ(std::vector<int32_t>()->*BooleanString, "False");
  EXPECT_EQ(std::vector<int32_t>({ 1, 2, 3, 4, 5, 6, 7 })->*BooleanString, "True");
  EXPECT_EQ(std::optional<int32_t>()->*BooleanString, "False");
  EXPECT_EQ(std::optional<int32_t>(0)->*BooleanString, "True");
  EXPECT_EQ(std::unique_ptr<int32_t>(new int32_t)->*BooleanString, "True");
  EXPECT_EQ(std::unique_ptr<int32_t>()->*BooleanString, "False");
  EXPECT_EQ(nullptr->*BooleanString, "False");
  EXPECT_EQ(std::nullopt->*BooleanString, "True");

  int32_t* PointerValue1 = new int32_t{ 1 };
  EXPECT_EQ(PointerValue1->*BooleanString, "True");
  delete PointerValue1;

  int32_t* PointerValue0 = new int32_t{ 0 };
  EXPECT_EQ(PointerValue0->*BooleanString, "False");
  delete PointerValue0;

  int32_t* PointerNull = nullptr;
  EXPECT_EQ(PointerNull->*BooleanString, "False");
  delete PointerNull;
}

TEST(ExtensionMethodTest, MethodExtension)
{
  std::string Value = "Denis West";
  Value->*ShowLog();
  Value->*Print();

  EXPECT_EQ(Value->*Append(" Williams")->*UpperCase->*Count, 19);

  Value->*As<std::vector<int32_t>>()->*Print();
  Value->*To<std::vector<int32_t>>->*Select([](const int32_t Value) { return Value * 2; })->*Print();
  Value->*To<std::vector<int32_t>>->*Select([](const int32_t Value) { return Value * 2; })->*To<std::list<double>>->*Print();

  EXPECT_EQ(10->*ToString(), "10");
  EXPECT_EQ(15.3625->*ToString(), "15.362500");
}
