#include <gtest/gtest.h>
#include <gmock/gmock.h>

auto main(int32_t argc, char** argv) -> int32_t
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}