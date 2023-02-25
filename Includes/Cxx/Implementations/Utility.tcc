template <typename T>
[[nodiscard]] inline constexpr decltype(auto) Cxx::Utilities::deep_raw_pointer_dereference(T&& pointer_or_value) noexcept
{
  if constexpr ( requires { *pointer_or_value; } )
  {
    return deep_raw_pointer_dereference(*pointer_or_value);
  }
  else
  {
    return pointer_or_value;
  }
}

template <typename T>
[[nodiscard]] inline constexpr decltype(auto) Cxx::Utilities::deep_smart_pointer_dereference(T&& pointer_or_value) noexcept
{
  if constexpr ( requires { *pointer_or_value; } )
  {
    return deep_smart_pointer_dereference(*pointer_or_value);
  }
  else if constexpr ( requires { *pointer_or_value.get(); } )
  {
    return deep_smart_pointer_dereference(*pointer_or_value.get());
  }
  else
  {
    return pointer_or_value;
  }
}

template <typename LeftType, typename RightType>
inline constexpr auto Cxx::Details::CustomizationPointObjects::CompareThreeWayOrderFallback::operator()(LeftType&& left, RightType&& right) const noexcept -> Traits::common_comparison_category_t<LeftType, RightType>
{
  using comparison_category_t = Traits::common_comparison_category_t<LeftType, RightType>;

  if constexpr ( std::same_as<comparison_category_t, std::strong_ordering> )
  {
    return std::compare_strong_order_fallback(std::forward<LeftType>(left), std::forward<RightType>(right));
  }
  else if constexpr ( std::same_as<comparison_category_t, std::partial_ordering> )
  {
    return std::compare_partial_order_fallback(std::forward<LeftType>(left), std::forward<RightType>(right));
  }
  else if constexpr ( std::same_as<comparison_category_t, std::weak_ordering> )
  {
    return std::compare_weak_order_fallback(std::forward<LeftType>(left), std::forward<RightType>(right));
  }
  else
  {
    static_assert(Traits::AlwaysFalse<LeftType>, "3-way comparison not available.");
  }
}

template <Cxx::Concepts::Character CharType>
inline constexpr bool Cxx::zstring_sentinel::operator==(const CharType* pointer) const noexcept
{
  return *pointer == CharType{};
}

template <Cxx::Concepts::Character CharType>
inline constexpr bool Cxx::zstring_sentinel::operator!=(const CharType* pointer) const noexcept
{
  return *pointer != CharType{};
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::ZString<CharType>::ZString(const_pointer pointer) noexcept
  : m_Pointer{ pointer }
  , m_Size{ std::char_traits<CharType>::length(pointer) }
{
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::ZString<CharType>::ZString(const_pointer pointer, const size_type size) noexcept
  : m_Pointer{ pointer }
  , m_Size{ size }
{
}

template <Cxx::Concepts::Character CharType>
template <size_t Size>
constexpr Cxx::ZString<CharType>::ZString(const std::type_identity_t<value_type> (&array)[Size]) noexcept
  : m_Pointer{ array }
  , m_Size{ Size }
{
}

template <Cxx::Concepts::Character CharType>
template <typename TraitType, typename AllocType>
constexpr Cxx::ZString<CharType>::ZString(const std::basic_string<CharType, TraitType, AllocType>& string) noexcept
  : m_Pointer{ string.data() }
  , m_Size{ string.size() }
{
}

template <Cxx::Concepts::Character CharType>
template <typename TraitType>
constexpr Cxx::ZString<CharType>::ZString(const std::basic_string_view<CharType, TraitType> string_view) noexcept
  : m_Pointer{ string_view.data() }
  , m_Size{ string_view.size() }
{
}

template <Cxx::Concepts::Character CharType>
template <size_t Size>
constexpr Cxx::ZString<CharType>::ZString(const std::array<CharType, Size>& array) noexcept
  : m_Pointer{ array.data() }
  , m_Size{ array.size() }
{
}

template <Cxx::Concepts::Character CharType>
template <size_t Size>
constexpr Cxx::ZString<CharType>::ZString(const std::span<CharType, Size>& span) noexcept
  : m_Pointer{ span.data() }
  , m_Size{ span.size() }
{
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::ZString<CharType>::size_type Cxx::ZString<CharType>::size() const noexcept
{
  return m_Size;
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::ZString<CharType>::const_iterator Cxx::ZString<CharType>::begin() const noexcept
{
  return m_Pointer;
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::ZString<CharType>::const_iterator Cxx::ZString<CharType>::cbegin() const noexcept
{
  return m_Pointer;
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::ZString<CharType>::const_iterator Cxx::ZString<CharType>::end() const noexcept
{
  return m_Pointer + m_Size;
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::ZString<CharType>::const_iterator Cxx::ZString<CharType>::cend() const noexcept
{
  return m_Pointer + m_Size;
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::zstring_sentinel Cxx::ZString<CharType>::zend() const noexcept
{
  return {};
}

template <Cxx::Concepts::Character CharType>
constexpr Cxx::zstring_sentinel Cxx::ZString<CharType>::zcend() const noexcept
{
  return {};
}

namespace Cxx::Literals::StringLiterals
{
  [[nodiscard]] inline constexpr ZString<char> operator"" _zs(const char* string, const size_t length) noexcept
  {
    return ZString<char>{ string, length };
  }

  [[nodiscard]] inline constexpr ZString<wchar_t> operator"" _zs(const wchar_t* string, const size_t length) noexcept
  {
    return ZString<wchar_t>{ string, length };
  }

  [[nodiscard]] inline constexpr ZString<char8_t> operator"" _zs(const char8_t* string, const size_t length) noexcept
  {
    return ZString<char8_t>{ string, length };
  }

  [[nodiscard]] inline constexpr ZString<char16_t> operator"" _zs(const char16_t* string, const size_t length) noexcept
  {
    return ZString<char16_t>{ string, length };
  }

  [[nodiscard]] inline constexpr ZString<char32_t> operator"" _zs(const char32_t* string, const size_t length) noexcept
  {
    return ZString<char32_t>{ string, length };
  }
} // namespace Cxx::Literals::StringLiterals
