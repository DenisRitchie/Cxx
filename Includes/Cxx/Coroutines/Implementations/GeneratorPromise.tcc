namespace Cxx::Coroutines
{
  namespace Details
  {
    template <class>
    inline constexpr bool always_false = false; // false value attached to a dependent name (for static_assert)
  }

  template <typename Type, typename Alloc>
  Generator<Type, Alloc> Generator<Type, Alloc>::promise_type::get_return_object_on_allocation_failure() noexcept
  {
    return {};
  }

  template <typename Type, typename Alloc>
  Generator<Type, Alloc> Generator<Type, Alloc>::promise_type::get_return_object() noexcept
  {
    return Generator<Type, Alloc>(*this);
  }

  template <typename Type, typename Alloc>
  std::suspend_always Generator<Type, Alloc>::promise_type::initial_suspend() const noexcept
  {
    return {};
  }

  template <typename Type, typename Alloc>
  std::suspend_always Generator<Type, Alloc>::promise_type::final_suspend() const noexcept
  {
    return {};
  }

  template <typename Type, typename Alloc>
  std::suspend_always Generator<Type, Alloc>::promise_type::yield_value(const Type& value) noexcept
  {
    m_value = std::addressof(value);
    return {};
  }

  template <typename Type, typename Alloc>
  void Generator<Type, Alloc>::promise_type::return_void() const noexcept
  {
  }

  template <typename Type, typename Alloc>
  void Generator<Type, Alloc>::promise_type::unhandled_exception() noexcept
  {
    m_exception = std::current_exception();
  }

  template <typename Type, typename Alloc>
  void Generator<Type, Alloc>::promise_type::rethrow_if_exception()
  {
    if ( m_exception )
    {
      std::rethrow_exception(m_exception);
    }
  }

  template <typename Type, typename Alloc>
  template <class U>
  U&& Generator<Type, Alloc>::promise_type::await_transform(U&& whatever) noexcept
  {
    static_assert(Details::always_false<U>, "co_await is not supported in coroutines of type Cxx::Coroutines::Generator");
    return std::forward<U>(whatever);
  }

  template <typename Type, typename Alloc>
  void* Generator<Type, Alloc>::promise_type::operator new(std::size_t size) noexcept
  {
    alloc_byte alloc{};
    return std::allocator_traits<alloc_byte>::allocate(alloc, size);
  }

  template <typename Type, typename Alloc>
  void Generator<Type, Alloc>::promise_type::operator delete(void* pointer, std::size_t size) noexcept
  {
    alloc_byte alloc{};
    return std::allocator_traits<alloc_byte>::deallocate(alloc, static_cast<std::byte*>(pointer), size);
  }
} // namespace Cxx::Coroutines
