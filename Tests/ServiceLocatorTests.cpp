#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ServiceLocator.hpp"

TEST(ServiceLocatorTest, GetInstance)
{
  using namespace std;
  using namespace NativeDesignPatterns;

  ServiceLocator::Default().Register<std::string>("String"s);
  EXPECT_EQ(ServiceLocator::Default().GetService<std::string>(), "String"s);

  EXPECT_THROW(ServiceLocator::Default().GetService<int32_t>(), std::logic_error);

  ServiceLocator::Default().GetService<std::string>() = "New String";
  EXPECT_EQ(ServiceLocator::Default().GetService<std::string>(), "New String"s);
}

TEST(ServiceLocatorTest, GetInstanceOrDefault)
{
  using namespace std;
  using namespace NativeDesignPatterns;

  ServiceLocator::Default().Register<std::string>("String"s);
  EXPECT_TRUE(ServiceLocator::Default().GetServiceOrDefault<std::string>().has_value());

  EXPECT_EQ(*ServiceLocator::Default().GetServiceOrDefault<std::string>(), "String"s);
  EXPECT_EQ(ServiceLocator::Default().GetServiceOrDefault<std::string>().operator*(), "String"s);
  EXPECT_STREQ(ServiceLocator::Default().GetServiceOrDefault<std::string>()->c_str(), "String");

  EXPECT_NO_THROW(ServiceLocator::Default().GetServiceOrDefault<int32_t>());
}
