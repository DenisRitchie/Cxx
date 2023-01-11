#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <any>
#include <tuple>
#include <vector>
#include <string>

#include "Cxx/TypeTraits.hpp"

using std::any;
using std::string;
using std::tuple;
using std::vector;

using Cxx::Traits::NumberOfArguments;
using Cxx::Traits::RemoveAllSymbols;
using Cxx::Traits::TemplateTraits;
using Cxx::Traits::TypeParameters::Arg;
using Cxx::Traits::TypeParameters::ArgumentIndex;
using Cxx::Traits::TypeParameters::ArgumentIndexType;
using Cxx::Traits::TypeParameters::Arguments;
using Cxx::Traits::TypeParameters::ReplaceArguments;
using Cxx::Traits::TypeParameters::TemplateArguments;

TEST(TypeTraitsTests, TypeParameters_V2_ArgumentIndexType)
{
  ArgumentIndexType<0, Arguments<int, char, double, float>> v0;
  ArgumentIndexType<1, Arguments<int, char, double, float>> v1;
  ArgumentIndexType<2, Arguments<int, char, double, float>> v2;
  ArgumentIndexType<3, Arguments<int, char, double, float>> v3;
  Arguments<int, char, double, float>::ArgumentIndex<0>     v4;
  Arguments<int, char, double, float>::ArgumentIndex<1>     v5;
  Arguments<int, char, double, float>::ArgumentIndex<2>     v6;
  Arguments<int, char, double, float>::ArgumentIndex<3>     v7;
  EXPECT_EQ(typeid(v0), typeid(v4));
  EXPECT_EQ(typeid(v1), typeid(v5));
  EXPECT_EQ(typeid(v2), typeid(v6));
  EXPECT_EQ(typeid(v3), typeid(v7));
  EXPECT_EQ(typeid(v0), typeid(int));
  EXPECT_EQ(typeid(v1), typeid(char));
  EXPECT_EQ(typeid(v2), typeid(double));
  EXPECT_EQ(typeid(v3), typeid(float));
  EXPECT_EQ(typeid(v4), typeid(int));
  EXPECT_EQ(typeid(v5), typeid(char));
  EXPECT_EQ(typeid(v6), typeid(double));
  EXPECT_EQ(typeid(v7), typeid(float));
}

TEST(TypeTraitsTests, TypeParameters_V2_TemplateArguments)
{
  using Tuple  = tuple<int32_t, int32_t, int32_t, int32_t, int32_t, int32_t>;
  using Vector = vector<int32_t>;

  EXPECT_EQ(TemplateArguments<any>::ArgumentCount, NumberOfArguments<0>);
  EXPECT_EQ(TemplateArguments<int32_t>::ArgumentCount, NumberOfArguments<0>);

  EXPECT_EQ(TemplateArguments<Vector>::ArgumentCount, NumberOfArguments<2>);
  EXPECT_EQ(typeid(TemplateArguments<Vector>::ArgumentIndex<0>), typeid(Vector::value_type));
  EXPECT_EQ(typeid(TemplateArguments<Vector>::ArgumentIndex<1>), typeid(Vector::allocator_type));
  EXPECT_EQ(typeid(TemplateArguments<TemplateArguments<Vector>::ArgumentIndex<1>>::ArgumentIndex<0>), typeid(Vector::allocator_type::value_type));

  EXPECT_EQ(TemplateArguments<string>::ArgumentCount, NumberOfArguments<3>);
  EXPECT_EQ(typeid(TemplateArguments<string>::ArgumentIndex<0>), typeid(string::value_type));
  EXPECT_EQ(typeid(TemplateArguments<string>::ArgumentIndex<1>), typeid(string::traits_type));
  EXPECT_EQ(typeid(TemplateArguments<TemplateArguments<string>::ArgumentIndex<1>>::ArgumentIndex<0>), typeid(string::traits_type::char_type));
  EXPECT_EQ(typeid(TemplateArguments<string>::ArgumentIndex<2>), typeid(string::allocator_type));
  EXPECT_EQ(typeid(TemplateArguments<TemplateArguments<string>::ArgumentIndex<2>>::ArgumentIndex<0>), typeid(string::allocator_type::value_type));

  EXPECT_EQ(TemplateArguments<Tuple>::ArgumentCount, NumberOfArguments<6>);
  []<size_t... Index>(std::index_sequence<Index...>)
  {
    EXPECT_TRUE((std::same_as<TemplateArguments<Tuple>::ArgumentIndex<Index>, std::tuple_element_t<Index, Tuple>> && ...));
  }
  (std::make_index_sequence<TemplateArguments<Tuple>::ArgumentCount>{});
}

TEST(TypeTraitsTests, TypeParameters_V2_ReplaceArguments)
{
  constexpr size_t IndexNotFound = static_cast<size_t>(-1);

  using NotFound = Cxx::Traits::TypeParameters::V2::Details::NotFound;
  using Tuple    = tuple<int8_t, int16_t, int32_t, int64_t, float, double>;

  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::TemplateArgs::ArgumentIndex<0>::Type, int8_t>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::TemplateArgs::ArgumentIndex<1>::Type, int16_t>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::TemplateArgs::ArgumentIndex<2>::Type, int32_t>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::TemplateArgs::ArgumentIndex<3>::Type, int64_t>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::TemplateArgs::ArgumentIndex<4>::Type, float>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::TemplateArgs::ArgumentIndex<5>::Type, double>));

  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::ReplaceArgs::ArgumentIndex<0>::Type, char8_t>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::ReplaceArgs::ArgumentIndex<1>::Type, char16_t>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::ReplaceArgs::ArgumentIndex<2>::Type, char32_t>));

  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<0>::Type, NotFound>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<1>::Type, char8_t>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<2>::Type, NotFound>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<3>::Type, char16_t>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<4>::Type, NotFound>));
  EXPECT_TRUE((std::same_as<ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<5>::Type, char32_t>));

  EXPECT_EQ((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<0>::SearchIndex()), IndexNotFound);
  EXPECT_EQ((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<1>::SearchIndex()), 0);
  EXPECT_EQ((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<2>::SearchIndex()), IndexNotFound);
  EXPECT_EQ((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<3>::SearchIndex()), 1);
  EXPECT_EQ((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<4>::SearchIndex()), IndexNotFound);
  EXPECT_EQ((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<5>::SearchIndex()), 2);

  EXPECT_FALSE((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<0>::Exists));
  EXPECT_TRUE((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<1>::Exists));
  EXPECT_FALSE((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<2>::Exists));
  EXPECT_TRUE((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<3>::Exists));
  EXPECT_FALSE((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<4>::Exists));
  EXPECT_TRUE((ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::FindIndexArgs::ArgumentIndex<5>::Exists));

  using NewTuple = ReplaceArguments<Tuple, Arg<1, char8_t>, Arg<3, char16_t>, Arg<5, char32_t>>::Type;

  EXPECT_EQ(typeid(std::tuple_element_t<0, Tuple>), typeid(std::tuple_element_t<0, NewTuple>));
  EXPECT_NE(typeid(std::tuple_element_t<1, Tuple>), typeid(std::tuple_element_t<1, NewTuple>));
  EXPECT_EQ(typeid(std::tuple_element_t<2, Tuple>), typeid(std::tuple_element_t<2, NewTuple>));
  EXPECT_NE(typeid(std::tuple_element_t<3, Tuple>), typeid(std::tuple_element_t<3, NewTuple>));
  EXPECT_EQ(typeid(std::tuple_element_t<4, Tuple>), typeid(std::tuple_element_t<4, NewTuple>));
  EXPECT_NE(typeid(std::tuple_element_t<5, Tuple>), typeid(std::tuple_element_t<5, NewTuple>));

  EXPECT_EQ(typeid(std::tuple_element_t<0, NewTuple>), typeid(int8_t));
  EXPECT_EQ(typeid(std::tuple_element_t<1, NewTuple>), typeid(char8_t));
  EXPECT_EQ(typeid(std::tuple_element_t<2, NewTuple>), typeid(int32_t));
  EXPECT_EQ(typeid(std::tuple_element_t<3, NewTuple>), typeid(char16_t));
  EXPECT_EQ(typeid(std::tuple_element_t<4, NewTuple>), typeid(float));
  EXPECT_EQ(typeid(std::tuple_element_t<5, NewTuple>), typeid(char32_t));
}

