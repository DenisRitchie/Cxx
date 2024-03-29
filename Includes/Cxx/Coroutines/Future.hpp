#ifndef E3DB091A_9250_46AB_9955_2DCE898CF92B
#define E3DB091A_9250_46AB_9955_2DCE898CF92B

#include <chrono>
#include <concepts>
#include <coroutine>
#include <future>
#include <thread>

// https://en.cppreference.com/w/cpp/coroutine/coroutine_traits

namespace Cxx::Coroutines
{
  struct as_coroutine
  {
  };
} // namespace Cxx::Coroutines

// Enable the use of std::future<T> as a coroutine type
// by using a std::promise<T> as the promise type.
template <typename T, typename... Args>
requires(!std::is_void_v<T> && !std::is_reference_v<T>)
struct std::coroutine_traits<std::future<T>, Args...>
{
    struct promise_type : std::promise<T>
    {
        std::future<T> get_return_object() noexcept
        {
          return this->get_future();
        }

        std::suspend_never initial_suspend() const noexcept
        {
          return {};
        }

        std::suspend_never final_suspend() const noexcept
        {
          return {};
        }

        void return_value(const T& value) noexcept(std::is_nothrow_copy_constructible_v<T>)
        {
          this->set_value(value);
        }

        void return_value(T&& value) noexcept(std::is_nothrow_move_constructible_v<T>)
        {
          this->set_value(std::move(value));
        }

        void unhandled_exception() noexcept
        {
          this->set_exception(std::current_exception());
        }
    };
};

// Same for std::future<void>.
template <typename... Args>
struct std::coroutine_traits<std::future<void>, Args...>
{
    struct promise_type : std::promise<void>
    {
        std::future<void> get_return_object() noexcept
        {
          return this->get_future();
        }

        std::suspend_never initial_suspend() const noexcept
        {
          return {};
        }

        std::suspend_never final_suspend() const noexcept
        {
          return {};
        }

        void return_void() noexcept
        {
          this->set_value();
        }

        void unhandled_exception() noexcept
        {
          this->set_exception(std::current_exception());
        }
    };
};

// Allow co_await'ing std::future<T> and std::future<void>
// by naively spawning a new thread for each co_await.
template <typename T>
requires(!std::is_reference_v<T>)
auto operator co_await(std::future<T> future) noexcept
{
  struct awaiter : std::future<T>
  {
      bool await_ready() const noexcept
      {
        using namespace std::chrono_literals;
        return this->wait_for(0s) != std::future_status::timeout;
      }

      void await_suspend(std::coroutine_handle<> handle) const
      {
        std::thread(
          [this, handle]
          {
            this->wait();
            handle.resume();
          }
        ).detach();
      }

      T await_resume()
      {
        return this->get();
      }
  };

  return awaiter{ std::move(future) };
}

#endif /* E3DB091A_9250_46AB_9955_2DCE898CF92B */
