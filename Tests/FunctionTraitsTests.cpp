#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <any>
#include <string>
#include <vector>

#include "Cxx/FunctionTraits.hpp"

TEST(FunctionTraitsTests, StdFunction)
{
  using FunctionType = std::function<std::string(int32_t, std::vector<std::any>)>;
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::ReturnType), typeid(std::string));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Argument<0>), typeid(int32_t));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Argument<1>), typeid(std::vector<std::any>));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Pointer), typeid(std::string(*)(int32_t, std::vector<std::any>)));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Function), typeid(FunctionType));
  EXPECT_EQ(typeid(Cxx::Traits::TypeParameters::TemplateArguments<Cxx::Traits::FunctionTraits<FunctionType>::ReturnType>::ArgumentIndex<0>), typeid(std::string::value_type));
  EXPECT_EQ(typeid(Cxx::Traits::TypeParameters::TemplateArguments<Cxx::Traits::FunctionTraits<FunctionType>::ReturnType>::ArgumentIndex<1>), typeid(std::string::traits_type));
  EXPECT_EQ(typeid(Cxx::Traits::TypeParameters::TemplateArguments<Cxx::Traits::FunctionTraits<FunctionType>::ReturnType>::ArgumentIndex<2>), typeid(std::string::allocator_type));
  EXPECT_EQ(typeid(Cxx::Traits::TemplateTraits<Cxx::Traits::FunctionTraits<FunctionType>::Argument<1>>::ElementType), typeid(std::any));
}
