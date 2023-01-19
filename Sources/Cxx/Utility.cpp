#include "Cxx/Utility.hpp"

namespace Cxx
{
  inline constexpr bool ZStringSentinel::operator==(const char* pointer) const noexcept
  {
    return *pointer == '\0';
  }

  inline constexpr bool ZStringSentinel::operator==(const int8_t* pointer) const noexcept
  {
    return *pointer == int8_t{ '\0' };
  }

  inline constexpr bool ZStringSentinel::operator==(const uint8_t* pointer) const noexcept
  {
    return *pointer == uint8_t{ '\0' };
  }

  inline constexpr bool ZStringSentinel::operator==(const uint16_t* pointer) const noexcept
  {
    return *pointer == uint16_t{ '\0' };
  }

  inline constexpr bool ZStringSentinel::operator==(const uint32_t* pointer) const noexcept
  {
    return *pointer == uint32_t{ '\0' };
  }

  inline constexpr bool ZStringSentinel::operator==(const wchar_t* pointer) const noexcept
  {
    return *pointer == L'\0';
  }

  inline constexpr bool ZStringSentinel::operator==(const char8_t* pointer) const noexcept
  {
    return *pointer == u8'\0';
  }

  inline constexpr bool ZStringSentinel::operator==(const char16_t* pointer) const noexcept
  {
    return *pointer == u'\0';
  }

  inline constexpr bool ZStringSentinel::operator==(const char32_t* pointer) const noexcept
  {
    return *pointer == U'\0';
  }

  inline constexpr bool ZStringSentinel::operator==(const std::byte* pointer) const noexcept
  {
    return *pointer == std::byte{ '\0' };
  }
} // namespace Cxx
