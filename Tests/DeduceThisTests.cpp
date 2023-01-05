//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING

#pragma warning(push)
#pragma warning(disable : 4996)

#include <__msvc_all_public_headers.hpp>

#pragma warning(pop)

struct MyTestClass;

struct MyTestStruct
{
    void ExternalMethod(this const MyTestClass& Self)
    {
      (void)Self;
      printf_s("\n----> Incocando: %s\n\n", __FUNCTION__);
    }

    void Test1(this const MyTestStruct&)
    {
      printf_s("\n----> Incocando: %s\n\n", __FUNCTION__);
    }

    void Test2()
    {
      Test1();
    }
};

struct MyTestClass
{
    MyTestClass(const MyTestStruct&)
    {
    }
};

TEST(DeduceThisTest, DeduceThis)
{
  // MyTestClass  object2;
  // object2.ExternalMethod();

  MyTestStruct object1;
  object1.ExternalMethod();
}
