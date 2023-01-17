#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cxx/Coroutines/Future.hpp"

class CoroutinesTests_SuspendAlways_Test;

template <typename... Args>
struct std::coroutine_traits<void, CoroutinesTests_SuspendAlways_Test&, Args...>
{
    // clang-format off

    struct promise_type
    {
        void get_return_object() const noexcept { }
        std::suspend_never initial_suspend() const noexcept { return { }; }
        std::suspend_never final_suspend() const noexcept { return { }; }
        void return_void() const noexcept {  }
        void unhandled_exception() const { throw; }
    };

    // clang-format on
};

std::future<int32_t> ComputeAsync()
{
  int32_t value1 = co_await std::async([] { return 6; });
  int32_t value2 = co_await std::async([] { return 7; });
  co_return value1 + value2;
}

TEST(CoroutinesTests, SuspendAlways)
{
  EXPECT_EQ((co_await ComputeAsync()), 13);
}
