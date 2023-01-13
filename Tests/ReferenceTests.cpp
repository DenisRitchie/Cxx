#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cxx/Optional.hpp"
#include "Cxx/Reference.hpp"
#include "Cxx/TypeTraits.hpp"

using Cxx::Optional;
using Cxx::Reference;

TEST(ReferenceTests, ChainInvocationArrowOperator)
{
  int32_t                         value = 10;
  std::reference_wrapper<int32_t> ref_value{ value };
  EXPECT_EQ(value, ref_value.get());

  ref_value.get() = 45;
  EXPECT_EQ(value, ref_value.get());

  int32_t*                         ptr_value;
  std::reference_wrapper<int32_t*> ref_ptr_value{ ptr_value };
  ref_ptr_value.get() = &value;

  EXPECT_EQ(&value, ptr_value);
  *ptr_value = 90;

  EXPECT_EQ(ref_value.get(), *ref_ptr_value.get());
  EXPECT_EQ(&ref_value.get(), ref_ptr_value.get());
}
