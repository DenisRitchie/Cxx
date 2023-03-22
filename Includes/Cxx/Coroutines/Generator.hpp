#ifndef AA77ADD6_B5B4_40A9_9DCB_EBBAC4AC5AB2
#define AA77ADD6_B5B4_40A9_9DCB_EBBAC4AC5AB2

#include <coroutine>
#include <stdexcept>
#include <memory>
#include <ranges>

// https://github.com/lewissbaker/cppcoro
// https://en.cppreference.com/w/cpp/language/coroutines
// https://en.cppreference.com/w/cpp/header/generator
// https://learn.microsoft.com/en-us/archive/msdn-magazine/2017/october/c-from-algorithms-to-coroutines-in-c

namespace Cxx::Coroutines
{
  template <typename Type, typename Alloc = std::allocator<std::byte>>
  class [[nodiscard]] Generator : public std::ranges::view_interface<Generator<Type, Alloc>>
  {
    public:
      struct promise_type
      {
          using alloc_byte = typename std::allocator_traits<Alloc>::template rebind_alloc<std::byte>;

          static_assert(std::is_same_v<std::byte*, typename std::allocator_traits<alloc_byte>::pointer>, "generator does not support allocators with fancy pointer types");
          static_assert(std::allocator_traits<alloc_byte>::is_always_equal::value && std::is_default_constructible_v<alloc_byte>, "generator supports only stateless allocators");

          const Type*        m_value;
          std::exception_ptr m_exception;

          static Generator    get_return_object_on_allocation_failure() noexcept;
          Generator           get_return_object() noexcept;
          std::suspend_always initial_suspend() const noexcept;
          std::suspend_always final_suspend() const noexcept;
          std::suspend_always yield_value(const Type& value) noexcept;
          void                return_void() const noexcept;
          void                unhandled_exception() noexcept;
          void                rethrow_if_exception();

          template <class U>
          U&& await_transform(U&& whatever) noexcept;

          static void* operator new(std::size_t size) noexcept;
          static void  operator delete(void* pointer, std::size_t size) noexcept;
      };

    public:
      using handle_type     = std::coroutine_handle<promise_type>;
      using value_type      = std::remove_cvref_t<Type>;
      using reference       = value_type&;
      using const_reference = const value_type&;
      using difference_type = std::ptrdiff_t;
      using size_type       = std::size_t;

    public:
      struct InputIterator
      {
          using value_type        = std::remove_cvref_t<Type>;
          using pointer           = value_type*;
          using const_pointer     = const value_type*;
          using reference         = value_type&;
          using const_reference   = const value_type&;
          using difference_type   = std::ptrdiff_t;
          using iterator_category = std::input_iterator_tag;
          using iterator_concept  = std::input_iterator_tag;

          handle_type m_handle{ nullptr };

          InputIterator() = default;
          explicit InputIterator(handle_type handle) noexcept;
          void                          operator++(int);
          InputIterator&                operator++();
          [[nodiscard]] const_reference operator*() const noexcept;
          [[nodiscard]] const_pointer   operator->() const noexcept;
          [[nodiscard]] bool            operator==(const InputIterator& right) const noexcept;
          [[nodiscard]] bool            operator!=(const InputIterator& right) const noexcept;
      };

      using iterator       = InputIterator;
      using const_iterator = InputIterator;

      [[nodiscard]] iterator begin();
      [[nodiscard]] iterator end() noexcept;

      Generator() = default;
      explicit Generator(promise_type& promise) noexcept;
      Generator(Generator&& right) noexcept;
      Generator& operator=(Generator&& right) noexcept;
      ~Generator();

    private:
      handle_type m_handle{ nullptr };
  };
} // namespace Cxx::Coroutines

#include "Implementations/Generator.tcc"
#include "Implementations/GeneratorPromise.tcc"
#include "Implementations/GeneratorIterator.tcc"

#endif /* AA77ADD6_B5B4_40A9_9DCB_EBBAC4AC5AB2 */
