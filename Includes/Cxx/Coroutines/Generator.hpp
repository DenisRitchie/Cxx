#ifndef AA77ADD6_B5B4_40A9_9DCB_EBBAC4AC5AB2
#define AA77ADD6_B5B4_40A9_9DCB_EBBAC4AC5AB2

#include <concepts>
#include <coroutine>
#include <stdexcept>
#include <variant>
#include <ranges>

// https://github.com/lewissbaker/cppcoro
// https://en.cppreference.com/w/cpp/language/coroutines
// https://en.cppreference.com/w/cpp/header/generator
// https://learn.microsoft.com/en-us/archive/msdn-magazine/2017/october/c-from-algorithms-to-coroutines-in-c

namespace Cxx::Coroutines
{
  template <typename T>
  struct [[nodiscard]] Generator : public std::ranges::view_interface<Generator<T>>
  {
      struct iterator;

      using value_type      = std::remove_reference_t<T>;
      using reference       = value_type&;
      using const_reference = const value_type&;
      using iterator        = iterator;
      using const_iterator  = iterator;
      using difference_type = std::ptrdiff_t;
      using size_type       = std::size_t;

      struct promise_type
      {
          promise_type& get_return_object() noexcept
          {
            return *this;
          }

          std::suspend_always initial_suspend() const noexcept
          {
            return {};
          }

          std::suspend_always final_suspend() const noexcept
          {
            return {};
          }

          std::suspend_always yield_value(const T& other) noexcept
          {
            value = std::addressof(other);
            return {};
          }

          void return_void()
          {
          }

          template <typename Expression>
          Expression&& await_transform(Expression&& expression)
          {
            static_assert(sizeof(expression) == 0, "co_await is not supported in coroutines of type generator");
            return std::forward<Expression>(expression);
          }

          void unhandled_exception()
          {
            value = std::current_exception();
          }

          void rethrow_if_failed()
          {
            if ( value.index() == 1 )
            {
              std::rethrow_exception(std::get<1>(value));
            }
          }

          std::variant<const T*, std::exception_ptr> value;
      };

      Generator()                            = default;
      Generator(const Generator&)            = delete;
      Generator& operator=(const Generator&) = delete;

      Generator(promise_type& promise)
        : handle(handle_type::from_promise(promise))
      {
      }

      Generator(Generator&& other)
        : handle(other.handle)
      {
        other.handle = nullptr;
      }

      Generator& operator=(Generator&& other)
      {
        if ( this != &other )
        {
          handle       = other.handle;
          other.handle = nullptr;
        }

        return *this;
      }

      ~Generator()
      {
        if ( handle )
        {
          handle.destroy();
        }
      }

      using handle_type = std::coroutine_handle<promise_type>;

      handle_type handle{ nullptr };

      struct iterator
      {
          using value_type        = std::remove_reference_t<T>;
          using pointer           = value_type*;
          using const_pointer     = const value_type*;
          using reference         = value_type&;
          using const_reference   = const value_type&;
          using difference_type   = std::ptrdiff_t;
          using iterator_category = std::input_iterator_tag;
          using iterator_concept  = std::input_iterator_tag;

          bool operator==(const iterator& other) const
          {
            return handle == other.handle;
          }

          bool operator!=(const iterator& other) const
          {
            return !(*this == other);
          }

          void operator++(int)
          {
            (void)operator++();
          }

          iterator& operator++()
          {
            handle.resume();

            if ( handle.done() )
            {
              promise_type& promise = handle.promise();
              handle                = nullptr;
              promise.rethrow_if_failed();
            }

            return *this;
          }

          const T& operator*() const
          {
            return *std::get<0>(handle.promise().value);
          }

          const T* operator->() const
          {
            return std::addressof(operator*());
          }

          handle_type handle;
      };

      iterator begin()
      {
        if ( !handle )
        {
          return { nullptr };
        }

        handle.resume();

        if ( handle.done() )
        {
          handle.promise().rethrow_if_failed();
          return { nullptr };
        }

        return { handle };
      }

      iterator end()
      {
        return { nullptr };
      }
  };

} // namespace Cxx::Coroutines

#endif /* AA77ADD6_B5B4_40A9_9DCB_EBBAC4AC5AB2 */
