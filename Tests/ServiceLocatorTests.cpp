#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "ServiceLocator.hpp"

#include <chrono>
#include <string_view>
#include <vector>
#include <memory>

using namespace std;
using namespace NativeDesignPatterns;

SemanticValue<string> StringFactory()
{
  return "String Factory"s;
}

SemanticValue<int32_t> Int32Factory()
{
  return 154'000;
}

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

struct PersonFactory
{
    SemanticValue<IPerson> operator()() const noexcept
    {
      return Person{};
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
    SemanticValue<IService> operator()() const noexcept
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

  EXPECT_EQ(Locator.GetService<IPerson>().GetValue(), "Denis West"sv);
  EXPECT_EQ(Locator.GetService<IService>().GetValue(), "IService"sv);
  EXPECT_EQ(Locator.GetService<std::string>(), std::string(30, '-'));
  EXPECT_EQ(Locator.GetService<int32_t>(), 50);
  EXPECT_EQ(Locator.GetService<std::shared_ptr<std::string>>()->compare(std::string(30, '*')), 0);
  EXPECT_EQ(Locator.Resolve<std::shared_ptr<std::string>>()->compare(std::string(30, '*')), 0);

  Locator.Resolve<std::shared_ptr<std::string>>().operator->().operator->() = std::make_shared<std::string>("Hola");
  EXPECT_EQ(*Locator.Resolve<std::shared_ptr<std::string>>(), "Hola"s);

  Locator.GetService<int32_t>() += 100;
  EXPECT_EQ(*Locator.Resolve<int32_t>(), 150);
}

TEST(ServiceLocatorTest, ConstLocalInstance)
{
  const ServiceLocator Locator;
  EXPECT_THROW(Locator.GetService<IPerson>().GetValue(), std::logic_error);
  EXPECT_THROW(Locator.GetService<IService>().GetValue(), std::logic_error);
  EXPECT_THROW(Locator.GetService<std::string>(), std::logic_error);
  EXPECT_THROW(Locator.GetService<int32_t>(), std::logic_error);
  EXPECT_THROW([[maybe_unused]] auto Result = Locator.GetService<std::shared_ptr<std::string>>()->compare(std::string(30, '*')), std::logic_error);
  EXPECT_THROW([[maybe_unused]] auto Result = Locator.Resolve<std::shared_ptr<std::string>>()->compare(std::string(30, '*')), std::bad_optional_access);
  EXPECT_THROW(*Locator.Resolve<std::string>(), std::bad_optional_access);
  EXPECT_THROW(*Locator.Resolve<std::shared_ptr<std::string>>(), std::bad_optional_access);
  EXPECT_THROW(*Locator.Resolve<int32_t>(), std::bad_optional_access);
}

TEST(ServiceLocatorTest, UseFactoryAndValue)
{
  ServiceLocator::UseFactory<PersonFactory, ServiceFactory>(StringFactory, Int32Factory, []() -> SemanticValue<std::string_view> { return "Text"sv; });
  ServiceLocator::UseFactory([]() -> SemanticValue<std::vector<int32_t>> { return std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; });

  EXPECT_EQ(ServiceLocator::Value<IService>->GetValue(), "IService.Value"sv);
  EXPECT_EQ(ServiceLocator::Value<IPerson>->GetValue(), "Denis West"sv);
  EXPECT_EQ(*ServiceLocator::Value<int32_t>, 154'000);
  EXPECT_EQ(*ServiceLocator::Value<std::string>, "String Factory"s);
  EXPECT_EQ(ServiceLocator::Value<std::string>->compare("String Factory"), 0);

  for ( size_t Index = 0, ExpectValue = 1; Index < ServiceLocator::Value<std::vector<int32_t>>->size(); ++Index, ++ExpectValue )
  {
    EXPECT_EQ(ServiceLocator::Value<std::vector<int32_t>>[Index], ExpectValue);
    ServiceLocator::Value<std::vector<int32_t>>[Index] *= 10;
  }

  for ( size_t Index = 0, ExpectValue = 10; Index < ServiceLocator::Value<std::vector<int32_t>>->size(); ++Index, ExpectValue += 10 )
  {
    EXPECT_EQ(ServiceLocator::Value<std::vector<int32_t>>->at(Index), ExpectValue);
  }
}

TEST(ServiceLocatorTest, GlobalService)
{
  ServiceLocator::Default().Register<IPerson, Person>();
  ServiceLocator::Default().Register<IService, Service>("IService");
  ServiceLocator::Default().Register<std::string>(30, '-');
  ServiceLocator::Default().Register<int32_t>(50);
  ServiceLocator::Default().Register<std::shared_ptr<std::string>>([] { return std::make_shared<std::string>(30, '*'); });

  EXPECT_EQ(ServiceLocator::Default().GetService<IPerson>().GetValue(), "Denis West"sv);
  EXPECT_EQ(ServiceLocator::Default().GetService<IService>().GetValue(), "IService"sv);
  EXPECT_EQ(ServiceLocator::Default().GetService<std::string>(), std::string(30, '-'));
  EXPECT_EQ(ServiceLocator::Default().GetService<int32_t>(), 50);
  EXPECT_EQ(ServiceLocator::Default().GetService<std::shared_ptr<std::string>>()->compare(std::string(30, '*')), 0);
  EXPECT_EQ(ServiceLocator::Default().Resolve<std::shared_ptr<std::string>>()->compare(std::string(30, '*')), 0);

  ServiceLocator::Default().Resolve<std::shared_ptr<std::string>>().operator->().operator->() = std::make_shared<std::string>("Hola");
  EXPECT_EQ(*ServiceLocator::Default().Resolve<std::shared_ptr<std::string>>(), "Hola"s);

  ServiceLocator::Default().GetService<int32_t>() += 100;
  EXPECT_EQ(*ServiceLocator::Default().Resolve<int32_t>(), 150);

  ServiceLocator::Default().Register<std::string, std::string>("Test of Text");
  EXPECT_EQ(ServiceLocator::Default().GetService<std::string>().compare("Test of Text"), 0);
  EXPECT_EQ(ServiceLocator::Default().Resolve<std::string>()->compare("Test of Text"), 0);

  ServiceLocator::Default().Register<IService>([] { return Service{ "Example Services" }; });
  EXPECT_EQ(ServiceLocator::Default().GetService<IService>().GetValue(), "Example Services"sv);
  EXPECT_EQ(ServiceLocator::Default().Resolve<IService>()->GetValue(), "Example Services"sv);

  ServiceLocator::Default().Register<IService, Service>("Constructor Arguments Example");
  EXPECT_EQ(ServiceLocator::Default().GetService<IService>().GetValue(), "Constructor Arguments Example"sv);
  EXPECT_EQ(ServiceLocator::Default().Resolve<IService>().operator->().operator->()->GetValue(), "Constructor Arguments Example"sv);

  ServiceLocator::Default().InvokeFactory<PersonFactory, ServiceFactory>(
      StringFactory, Int32Factory, []() -> SemanticValue<std::string_view> { return "Text"sv; }
  );
  ServiceLocator::Default().InvokeFactory([]() -> SemanticValue<std::vector<int32_t>> { return std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; });

  EXPECT_EQ(ServiceLocator::Default().Resolve<IService>()->GetValue(), "IService.Value"sv);
  EXPECT_EQ(ServiceLocator::Default().Resolve<IPerson>()->GetValue(), "Denis West"sv);
  EXPECT_EQ(*ServiceLocator::Default().Resolve<int32_t>(), 154'000);
  EXPECT_EQ(*ServiceLocator::Default().Resolve<std::string>(), "String Factory"s);
  EXPECT_EQ(ServiceLocator::Default().Resolve<std::string>()->compare("String Factory"), 0);

  for ( size_t Index = 0, ExpectValue = 1; Index < ServiceLocator::Default().Resolve<std::vector<int32_t>>()->size(); ++Index, ++ExpectValue )
  {
    EXPECT_EQ(ServiceLocator::Default().Resolve<std::vector<int32_t>>()[Index], ExpectValue);
    ServiceLocator::Default().GetService<std::vector<int32_t>>()[Index] *= 10;
  }

  for ( size_t Index = 0, ExpectValue = 10; Index < ServiceLocator::Default().GetService<std::vector<int32_t>>().size(); ++Index, ExpectValue += 10 )
  {
    EXPECT_EQ(ServiceLocator::Default().Resolve<std::vector<int32_t>>()->at(Index), ExpectValue);
  }
}
