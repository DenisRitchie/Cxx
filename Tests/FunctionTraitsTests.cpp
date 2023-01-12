#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <any>
#include <string>
#include <vector>

#include "Cxx/FunctionTraits.hpp"

TEST(FunctionTraitsTests, StandardFunction)
{
  using FunctionType = std::function<std::string(int32_t, std::vector<std::any>)>;
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::ReturnType), typeid(std::string));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Argument<0>), typeid(int32_t));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Argument<1>), typeid(std::vector<std::any>));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Object), typeid(FunctionType));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Pointer), typeid(std::string(*)(int32_t, std::vector<std::any>)));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<FunctionType>::Function), typeid(FunctionType));
  EXPECT_EQ(typeid(Cxx::Traits::TypeParameters::TemplateArguments<Cxx::Traits::FunctionTraits<FunctionType>::ReturnType>::ArgumentIndex<0>), typeid(std::string::value_type));
  EXPECT_EQ(typeid(Cxx::Traits::TypeParameters::TemplateArguments<Cxx::Traits::FunctionTraits<FunctionType>::ReturnType>::ArgumentIndex<1>), typeid(std::string::traits_type));
  EXPECT_EQ(typeid(Cxx::Traits::TypeParameters::TemplateArguments<Cxx::Traits::FunctionTraits<FunctionType>::ReturnType>::ArgumentIndex<2>), typeid(std::string::allocator_type));
  EXPECT_EQ(typeid(Cxx::Traits::TemplateTraits<Cxx::Traits::FunctionTraits<FunctionType>::Argument<1>>::ElementType), typeid(std::any));
}

TEST(FunctionTraitsTests, Lambda)
{
  auto Lambda = [](int32_t, std::vector<std::vector<int32_t>>, std::u8string) -> std::any
  {
    return std::make_any<std::string>();
  };

  EXPECT_TRUE(Cxx::Traits::FunctionTraits<decltype(Lambda)>::HasArguments);
  EXPECT_EQ(Cxx::Traits::FunctionTraits<decltype(Lambda)>::ArgumentCount, 3);
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<decltype(Lambda)>::ReturnType), typeid(std::any));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<decltype(Lambda)>::Object), typeid(decltype(Lambda)));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<decltype(Lambda)>::Pointer), typeid(std::any(*)(int32_t, std::vector<std::vector<int32_t>>, std::u8string)));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<decltype(Lambda)>::Function), typeid(std::function<std::any(int32_t, std::vector<std::vector<int32_t>>, std::u8string)>));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<decltype(Lambda)>::Arguments), typeid(Cxx::Traits::TypeParameters::Arguments<int32_t, std::vector<std::vector<int32_t>>, std::u8string>));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<decltype(Lambda)>::Argument<0>), typeid(int32_t));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<decltype(Lambda)>::Argument<1>), typeid(std::vector<std::vector<int32_t>>));
  EXPECT_EQ(typeid(Cxx::Traits::FunctionTraits<decltype(Lambda)>::Argument<2>), typeid(std::u8string));
}

TEST(FunctionTraitsTests, PointerFunction)
{
  using FunctionPointer = std::vector<int32_t> (*(*)(const void*, const void*))(std::any) noexcept;
  EXPECT_TRUE(Cxx::Traits::FunctionTraits<FunctionPointer>::IsPointerFunction);
  EXPECT_FALSE(Cxx::Traits::FunctionTraits<FunctionPointer>::IsNoexcept);
  EXPECT_EQ(Cxx::Traits::FunctionTraits<FunctionPointer>::ArgumentCount, 2);
  EXPECT_TRUE((std::same_as<Cxx::Traits::FunctionTraits<FunctionPointer>::Argument<0>, const void*>));
  EXPECT_TRUE((std::same_as<Cxx::Traits::FunctionTraits<FunctionPointer>::Argument<1>, const void*>));
  EXPECT_TRUE((std::same_as<Cxx::Traits::FunctionTraits<FunctionPointer>::ReturnType, std::vector<int32_t> (*)(std::any) noexcept>));
  EXPECT_TRUE(Cxx::Traits::FunctionTraits<Cxx::Traits::FunctionTraits<FunctionPointer>::ReturnType>::IsPointerFunction);
  EXPECT_TRUE(Cxx::Traits::FunctionTraits<Cxx::Traits::FunctionTraits<FunctionPointer>::ReturnType>::IsNoexcept);
  EXPECT_EQ(Cxx::Traits::FunctionTraits<Cxx::Traits::FunctionTraits<FunctionPointer>::ReturnType>::ArgumentCount, 1);
  EXPECT_TRUE((std::same_as<Cxx::Traits::FunctionTraits<Cxx::Traits::FunctionTraits<FunctionPointer>::ReturnType>::Argument<0>, std::any>));
  EXPECT_TRUE((std::same_as<Cxx::Traits::FunctionTraits<Cxx::Traits::FunctionTraits<FunctionPointer>::ReturnType>::ReturnType, std::vector<int32_t>>));
  EXPECT_TRUE((std::same_as<Cxx::Traits::TemplateTraits<Cxx::Traits::FunctionTraits<Cxx::Traits::FunctionTraits<FunctionPointer>::ReturnType>::ReturnType>::TypeParameter<1>, std::vector<int32_t>::allocator_type>));
}

TEST(FunctionTraitsTests, MemberMethod)
{
  struct Object
  {
  };

  using MethodType1  = std::string (Object::*)(std::any, const void*, std::nullptr_t);
  using MethodType2  = std::string (Object::*)(std::any, const void*, std::nullptr_t)&;
  using MethodType3  = std::string (Object::*)(std::any, const void*, std::nullptr_t)&&;
  using MethodType4  = std::string (Object::*)(std::any, const void*, std::nullptr_t) const;
  using MethodType5  = std::string (Object::*)(std::any, const void*, std::nullptr_t) const&;
  using MethodType6  = std::string (Object::*)(std::any, const void*, std::nullptr_t) const&&;
  using MethodType7  = std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile;
  using MethodType8  = std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile&;
  using MethodType9  = std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile&&;
  using MethodType10 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile;
  using MethodType11 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile&;
  using MethodType12 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile&&;
  using MethodType13 = std::string (Object::*)(std::any, const void*, std::nullptr_t) noexcept;
  using MethodType14 = std::string (Object::*)(std::any, const void*, std::nullptr_t)& noexcept;
  using MethodType15 = std::string (Object::*)(std::any, const void*, std::nullptr_t)&& noexcept;
  using MethodType16 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const noexcept;
  using MethodType17 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const& noexcept;
  using MethodType18 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const&& noexcept;
  using MethodType19 = std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile noexcept;
  using MethodType20 = std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile& noexcept;
  using MethodType21 = std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile&& noexcept;
  using MethodType22 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile noexcept;
  using MethodType23 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile& noexcept;
  using MethodType24 = std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile&& noexcept;
}
