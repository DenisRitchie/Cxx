namespace Cxx::Coroutines
{
  template <typename Type, typename Alloc>
  Generator<Type, Alloc>::InputIterator::InputIterator(handle_type handle) noexcept
    : m_handle{ handle }
  {
  }

  template <typename Type, typename Alloc>
  typename Generator<Type, Alloc>::InputIterator& Generator<Type, Alloc>::InputIterator::operator++()
  {
    m_handle.resume();

    if ( m_handle.done() )
    {
      std::exchange(m_handle, nullptr).promise().rethrow_if_exception();
    }

    return *this;
  }

  template <typename Type, typename Alloc>
  void Generator<Type, Alloc>::InputIterator::operator++(int)
  {
    // This operator meets the requirements of the C++20 input_iterator concept, but not the Cpp17InputIterator requirements.
    ++*this;
  }

  template <typename Type, typename Alloc>
  typename Generator<Type, Alloc>::InputIterator::const_reference Generator<Type, Alloc>::InputIterator::operator*() const noexcept
  {
    return *m_handle.promise().m_value;
  }

  template <typename Type, typename Alloc>
  typename Generator<Type, Alloc>::InputIterator::const_pointer Generator<Type, Alloc>::InputIterator::operator->() const noexcept
  {
    return m_handle.promise().m_value;
  }

  template <typename Type, typename Alloc>
  bool Generator<Type, Alloc>::InputIterator::operator==(const Generator<Type, Alloc>::InputIterator& right) const noexcept
  {
    return m_handle == right.m_handle;
  }

  template <typename Type, typename Alloc>
  bool Generator<Type, Alloc>::InputIterator::operator!=(const Generator<Type, Alloc>::InputIterator& right) const noexcept
  {
    return m_handle != right.m_handle;
  }
} // namespace Cxx::Coroutines
