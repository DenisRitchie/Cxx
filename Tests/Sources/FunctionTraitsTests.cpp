/**
 * @file FunctionTraitsTests.cpp
 * @author Denis West (DenisWest@outlook.com)
 * @brief Pruebas unitarias de Cxx::Traits::FunctionsTraits
 * @version 0.1
 * @date 2023-01-11
 * @copyright Copyright (c) 2023
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <any>
#include <string>
#include <sstream>
#include <vector>
#include <variant>

#include "Cxx/FunctionTraits.hpp"

using ::testing::Eq;
using ::testing::Ge;
using ::testing::Gt;
using ::testing::IsFalse;
using ::testing::IsNull;
using ::testing::IsTrue;
using ::testing::Le;
using ::testing::Lt;
using ::testing::Ne;
using ::testing::NotNull;
using ::testing::Optional;
using ::testing::Ref;
using ::testing::TypedEq;
using ::testing::VariantWith;

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

struct Object
{
};

// clang-format off

using MethodTypes = Cxx::Traits::TypeParameters::Arguments
<
  std::string (Object::*)(std::any, const void*, std::nullptr_t)                            ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const                      ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile                   ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile             ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) &                          ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const &                    ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile &                 ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile &           ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) &&                         ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const &&                   ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile &&                ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile &&          ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) noexcept                   ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const noexcept             ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile noexcept          ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile noexcept    ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) & noexcept                 ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const & noexcept           ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile & noexcept        ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile & noexcept  ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) && noexcept                ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const && noexcept          ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) volatile && noexcept       ,
  std::string (Object::*)(std::any, const void*, std::nullptr_t) const volatile && noexcept
>;

// clang-format on

//
// GoogleTest FAQ:             https://google.github.io/googletest/faq.html
// Advanced googletest Topics: https://google.github.io/googletest/advanced.html
//
TEST(FunctionTraitsTests, MemberFunction)
{
  size_t lvalues                   = 0;
  size_t lconsts                   = 0;
  size_t lvolatiles                = 0;
  size_t lconst_volatiles          = 0;
  size_t lnoexcepts                = 0;
  size_t lconst_noexcepts          = 0;
  size_t lvolatile_noexcepts       = 0;
  size_t lconst_volatile_noexcepts = 0;
  size_t rvalues                   = 0;
  size_t rconsts                   = 0;
  size_t rvolatiles                = 0;
  size_t rconst_volatiles          = 0;
  size_t rnoexcepts                = 0;
  size_t rconst_noexcepts          = 0;
  size_t rvolatile_noexcepts       = 0;
  size_t rconst_volatile_noexcepts = 0;
  size_t members                   = 0;
  size_t consts                    = 0;
  size_t volatiles                 = 0;
  size_t const_volatiles           = 0;
  size_t noexcepts                 = 0;
  size_t const_noexcepts           = 0;
  size_t volatile_noexcepts        = 0;
  size_t const_volatile_noexcepts  = 0;

  [&]<size_t... Index>(std::index_sequence<Index...>)
  {
    lvalues                   = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsL + ...);
    lconsts                   = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsLConst + ...);
    lvolatiles                = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsLVolatile + ...);
    lconst_volatiles          = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsLConstVolatile + ...);
    lnoexcepts                = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsLNoexcept + ...);
    lconst_noexcepts          = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsLConstNoexcept + ...);
    lvolatile_noexcepts       = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsLVolatileNoexcept + ...);
    lconst_volatile_noexcepts = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsLConstVolatileNoexcept + ...);
    rvalues                   = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsR + ...);
    rconsts                   = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsRConst + ...);
    rvolatiles                = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsRVolatile + ...);
    rconst_volatiles          = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsRConstVolatile + ...);
    rnoexcepts                = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsRNoexcept + ...);
    rconst_noexcepts          = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsRConstNoexcept + ...);
    rvolatile_noexcepts       = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsRVolatileNoexcept + ...);
    rconst_volatile_noexcepts = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsRConstVolatileNoexcept + ...);
    members                   = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsMemberFunction + ...);
    consts                    = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsConst + ...);
    volatiles                 = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsVolatile + ...);
    const_volatiles           = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsConstVolatile + ...);
    noexcepts                 = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsNoexcept + ...);
    const_noexcepts           = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsConstNoexcept + ...);
    volatile_noexcepts        = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsVolatileNoexcept + ...);
    const_volatile_noexcepts  = (Cxx::Traits::FunctionTraits<MethodTypes::ArgumentIndex<Index>>::IsConstVolatileNoexcept + ...);
  }
  (std::make_index_sequence<MethodTypes::ArgumentCount>{});

  EXPECT_THAT(lvalues, Eq(16));
  EXPECT_THAT(lconsts, Eq(8));
  EXPECT_THAT(lvolatiles, Eq(8));
  EXPECT_THAT(lconst_volatiles, Eq(4));
  EXPECT_THAT(lnoexcepts, Eq(8));
  EXPECT_THAT(lconst_noexcepts, Eq(4));
  EXPECT_THAT(lvolatile_noexcepts, Eq(4));
  EXPECT_THAT(lconst_volatile_noexcepts, Eq(2));

  EXPECT_THAT(rvalues, Eq(16));
  EXPECT_THAT(rconsts, Eq(8));
  EXPECT_THAT(rvolatiles, Eq(8));
  EXPECT_THAT(rconst_volatiles, Eq(4));
  EXPECT_THAT(rnoexcepts, Eq(8));
  EXPECT_THAT(rconst_noexcepts, Eq(4));
  EXPECT_THAT(rvolatile_noexcepts, Eq(4));
  EXPECT_THAT(rconst_volatile_noexcepts, Eq(2));

  EXPECT_THAT(members, Eq(24));
  EXPECT_THAT(consts, Eq(12));
  EXPECT_THAT(volatiles, Eq(12));
  EXPECT_THAT(const_volatiles, Eq(6));
  EXPECT_THAT(noexcepts, Eq(12));
  EXPECT_THAT(const_noexcepts, Eq(6));
  EXPECT_THAT(volatile_noexcepts, Eq(6));
  EXPECT_THAT(const_volatile_noexcepts, Eq(3));
}
