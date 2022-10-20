#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ServiceLocator.hpp"

#include <chrono>
#include <string_view>
#include <vector>
#include <memory>

using namespace std;
using namespace NativeDesignPatterns;

struct IPerson
{
    virtual std::string_view GetValue() const noexcept = 0;
};

struct Person : public IPerson
{
    virtual std::string_view GetValue() const noexcept override
    {
      return "Denis West";
    }
};

struct IService
{
    virtual std::string_view GetValue() const noexcept = 0;
};

struct Service : public IService
{
    constexpr Service(const std::string_view Value) noexcept
      : Value{ Value }
    {
    }

    virtual std::string_view GetValue() const noexcept override
    {
      return Value;
    }

    std::string_view Value;
};

struct ServiceFactory
{
    NativeDesignPatterns::SemanticValue<IService> operator()() const noexcept
    {
      return Service{ "IService.Value" };
    }
};

TEST(ServiceLocatorTest, LocalInstance)
{
  ServiceLocator Locator;
  Locator.Register<IPerson, Person>();
  Locator.Register<IService, Service>("IService");
  Locator.Register<std::string>(30, '-');
  Locator.Register<int32_t>(50);
  Locator.Register<std::shared_ptr<std::string>>([] { return std::make_shared<std::string>(30, '*'); });

  std::is_copy_constructible_v<Person>;
  std::is_move_constructible_v<Person>;

  EXPECT_EQ(Locator.GetService<IPerson>().GetValue(), "Denis West"sv);
  EXPECT_EQ(Locator.GetService<IService>().GetValue(), "IService"sv);
  EXPECT_EQ(Locator.Resolve<std::shared_ptr<std::string>>()->compare(std::string(30, '*')), 0);
}

TEST(ServiceLocatorTest, GetInstance)
{
  ServiceLocator::Default().Register<std::string>("String"s);
  EXPECT_EQ(ServiceLocator::Default().GetService<std::string>(), "String"s);

  EXPECT_THROW(ServiceLocator::Default().GetService<int32_t>(), std::logic_error);

  ServiceLocator::Default().GetService<std::string>() = "New String";
  EXPECT_EQ(ServiceLocator::Default().GetService<std::string>(), "New String"s);
}

TEST(ServiceLocatorTest, Resolve)
{
  using namespace std;
  using namespace NativeDesignPatterns;

  ServiceFactory F;
  F()->GetValue();

  ServiceLocator::UseFactory<ServiceFactory>();
  EXPECT_EQ(ServiceLocator::Value<IService>->GetValue(), "IService.Value"sv);

  ServiceLocator::Default().Register<std::string>("String"s);
  EXPECT_TRUE(ServiceLocator::Default().Resolve<std::string>().has_value());
  EXPECT_EQ(*ServiceLocator::Default().Resolve<std::string>(), "String"s);
  EXPECT_EQ(ServiceLocator::Default().Resolve<std::string>().operator*(), "String"s);

  EXPECT_STREQ(ServiceLocator::Default().Resolve<std::string>()->c_str(), "String");

  EXPECT_NO_THROW(ServiceLocator::Default().Resolve<int32_t>());

  // SemanticValue<std::string>                  S{ ""s };
  // SemanticValue<std::string>::type            T1;
  // SemanticValue<std::string>::reference       T2 = T1;
  // SemanticValue<std::string>::pointer         T3 = nullptr;
  // SemanticValue<std::string>::const_reference T4 = "Hola";
  // SemanticValue<std::string>::const_pointer   T5 = nullptr;

  const ServiceLocator    Locator;
  // Locator.GetService<std::string>() = "Hola";
  [[maybe_unused]] auto&& V1 = ServiceLocator::Value<std::string>->c_str();
  // Locator.Resolve<std::string>().operator->().operator->()->shrink_to_fit();
  [[maybe_unused]] auto&& V2 = Locator.Resolve<std::string>().operator->().operator->()->c_str();

  // std::optional<int32_t>                      O;
  // O.                                          operator=(10);
  // Optional<int32_t>                           O2;
  // O2.                                         operator=(O);
}
