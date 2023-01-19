

namespace Cxx::Coroutines
{
  template <typename Type, typename Alloc>
  Generator<Type, Alloc>::Generator(promise_type& promise) noexcept
    : m_handle(std::coroutine_handle<promise_type>::from_promise(promise))
  {
  }

  template <typename Type, typename Alloc>
  Generator<Type, Alloc>::Generator(Generator<Type, Alloc>&& right) noexcept
    : m_handle(std::exchange(right.m_handle, nullptr))
  {
  }

  template <typename Type, typename Alloc>
  Generator<Type, Alloc>& Generator<Type, Alloc>::operator=(Generator<Type, Alloc>&& right) noexcept
  {
    m_handle = std::exchange(right.m_handle, nullptr);
    return *this;
  }

  template <typename Type, typename Alloc>
  Generator<Type, Alloc>::~Generator()
  {
    if ( m_handle )
    {
      m_handle.destroy();
    }
  }

  template <typename Type, typename Alloc>
  Generator<Type, Alloc>::iterator Generator<Type, Alloc>::begin()
  {
    if ( m_handle )
    {
      m_handle.resume();

      if ( m_handle.done() )
      {
        m_handle.promise().rethrow_if_exception();
      }
    }

    return iterator{ m_handle };
  }

  template <typename Type, typename Alloc>
  Generator<Type, Alloc>::iterator Generator<Type, Alloc>::end() noexcept
  {
    return {};
  }
} // namespace Cxx::Coroutines
