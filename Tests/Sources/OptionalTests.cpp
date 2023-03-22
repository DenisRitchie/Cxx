#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>
#include <string_view>
#include <memory>
#include <ranges>

#include "Cxx/Optional.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

struct WrapperString
{
    bool operator==(const std::string_view text) const noexcept
    {
      return this->Text == text;
    }

#ifdef __cpp_explicit_this_parameter
    template <typename Self>
    decltype(auto) operator->(this Self&& This)
    {
      return std::addressof(This.Text);
    }
#else
    std::string* operator->()
    {
      return std::addressof(Text);
    }

    const std::string* operator->() const
    {
      return std::addressof(Text);
    }
#endif

    std::string Text;
};

struct TestModel
{
    constexpr const char& operator[](const int32_t index) const noexcept
    {
      return Value[index];
    }

    std::string_view Value;
};

struct Struct6
{
    TestModel* operator->()
    {
      return &Model;
    }

    TestModel Model;
};

struct Struct5
{
    Struct6& operator->()
    {
      return Struct;
    }

    Struct6 Struct;
};

struct Struct4
{
    Struct5& operator->()
    {
      return Struct;
    }

    Struct5 Struct;
};

struct Struct3
{
    Struct4& operator->()
    {
      return Struct;
    }

    Struct4 Struct;
};

struct Struct2
{
    Struct3& operator->()
    {
      return Struct;
    }

    Struct3 Struct;
};

struct Struct1
{
    Struct2& operator->()
    {
      return Struct;
    }

    Struct2 Struct;
};

struct Struct0
{
    Struct1& operator->()
    {
      return Struct;
    }

    Struct1 Struct;
};

TEST(OptionalTests, ChainInvocationArrowOperator)
{
  {
    Cxx::Optional<std::string> nullable_string = "Default Text";
    EXPECT_EQ(nullable_string->length(), 12);

    [[maybe_unused]] const Cxx::Optional<std::string> const_string = "Default Text";
    // const_string->assign("Other Text"); // Error: Const Object
  }

  {
    Cxx::Optional<WrapperString> nullable_string = WrapperString{ "Default Text" };
    // EXPECT_STREQ(nullable_string->c_str(), "Default Text");
  }

  {
    // clang-format off
    Struct0 data1;
    data1.operator->().operator->().operator->().operator->().operator->().operator->().operator->()->Value = "Test Data";

    Struct0 data2;
    data2.Struct.Struct.Struct.Struct.Struct.Struct.Model.Value = "Test Data";

    std::optional<Struct0> data3 = Struct0{};
    (*data3)->Value = "Test Data";

    std::optional<Struct0> data4 = Struct0{};
    data4->operator->()->Value = "Test Data";

    Cxx::Optional<Struct0> data5 = Struct0{};
    data5->Value = "Test Data";

    Cxx::Optional<Struct0> data6 = Struct0{};
    (*data6)->Value = "Test Data";

    Cxx::Optional<std::vector<int32_t>> data7 = std::vector<int32_t>{};
    data7->resize(5);
    data7[0] = 10;
    data7[1] = 20;
    data7[2] = 30;
    data7[3] = 40;
    data7[4] = 50;
    // clang-format on

    EXPECT_EQ(data1->Value, data2->Value);
    EXPECT_EQ((*data3)->Value, data4->operator->()->Value);
    EXPECT_EQ(data5->Value, data6->Value);

    for ( int32_t index = -1; const char letter : { 'T', 'e', 's', 't', ' ', 'D', 'a', 't', 'a' } )
    {
      ++index;

      EXPECT_EQ(data6.operator->().operator->().operator->().operator->().operator->().operator->().operator->().operator->()->operator[](index), letter);
      EXPECT_EQ(data6[index], letter);
    }

    for ( int32_t index = -1; const int32_t value                                                          //
                              : std::ranges::views::iota(1)                                                //
                                  | std::ranges::views::transform([](auto&& value) { return value * 10; }) //
                                  | std::ranges::views::take(5) )
    {
      EXPECT_EQ(data7[++index], value);
    }
  }
}
