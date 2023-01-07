#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <variant>

namespace Cxx
{
  class u8string : private std::basic_string<char8_t>
  {
    public:
      constexpr u8string() noexcept                                 = default;
      constexpr u8string(const u8string& other) noexcept            = default;
      constexpr u8string(u8string&& other) noexcept                 = default;
      constexpr u8string& operator=(const u8string& other) noexcept = default;
      constexpr u8string& operator=(u8string&& other) noexcept      = default;
      constexpr virtual ~u8string() noexcept                        = default;

    public:
      constexpr const u8string& operator=(const u8string& other) const noexcept;

    public:
      constexpr u8string(const char* text) noexcept;
      constexpr u8string(const wchar_t* text) noexcept;
      constexpr u8string(const char8_t* text) noexcept;
      constexpr u8string(const char16_t* text) noexcept;
      constexpr u8string(const char32_t* text) noexcept;
      constexpr u8string(const std::byte* text) noexcept;
      constexpr u8string(const int8_t* text) noexcept;
      constexpr u8string(const uint8_t* text) noexcept;
      constexpr u8string(const uint16_t* text) noexcept;
      constexpr u8string(const uint32_t* text) noexcept;

    public:
      constexpr u8string(const char* text, const size_t size) noexcept;
      constexpr u8string(const wchar_t* text, const size_t size) noexcept;
      constexpr u8string(const char8_t* text, const size_t size) noexcept;
      constexpr u8string(const char16_t* text, const size_t size) noexcept;
      constexpr u8string(const char32_t* text, const size_t size) noexcept;
      constexpr u8string(const std::byte* text, const size_t size) noexcept;
      constexpr u8string(const int8_t* text, const size_t size) noexcept;
      constexpr u8string(const uint8_t* text, const size_t size) noexcept;
      constexpr u8string(const uint16_t* text, const size_t size) noexcept;
      constexpr u8string(const uint32_t* text, const size_t size) noexcept;

    public:
      constexpr u8string(const char* first, const char* last) noexcept;
      constexpr u8string(const wchar_t* first, const char* last) noexcept;
      constexpr u8string(const char8_t* first, const char* last) noexcept;
      constexpr u8string(const char16_t* first, const char* last) noexcept;
      constexpr u8string(const char32_t* first, const char* lastt) noexcept;
      constexpr u8string(const std::byte* first, const char* last) noexcept;
      constexpr u8string(const int8_t* first, const char* last) noexcept;
      constexpr u8string(const uint8_t* first, const char* last) noexcept;
      constexpr u8string(const uint16_t* first, const char* last) noexcept;
      constexpr u8string(const uint32_t* first, const char* last) noexcept;

    public:
      constexpr u8string(const char letter, const size_t count) noexcept;
      constexpr u8string(const wchar_t letter, const size_t count) noexcept;
      constexpr u8string(const char8_t letter, const size_t count) noexcept;
      constexpr u8string(const char16_t letter, const size_t count) noexcept;
      constexpr u8string(const char32_t letter, const size_t count) noexcept;
      constexpr u8string(const std::byte letter, const size_t count) noexcept;
      constexpr u8string(const int8_t letter, const size_t count) noexcept;
      constexpr u8string(const uint8_t letter, const size_t count) noexcept;
      constexpr u8string(const uint16_t letter, const size_t count) noexcept;
      constexpr u8string(const uint32_t letter, const size_t count) noexcept;

    public:
      constexpr std::u8string_view view() const noexcept;

    public:
      constexpr std::u8string  utf8() const noexcept;
      constexpr std::u16string utf16() const noexcept;
      constexpr std::u32string utf32() const noexcept;

    public:
      // char8_t&          operator[](const int32_t index);
      // constexpr char8_t operator[](const int32_t index) const;
      using std::basic_string<char8_t>::operator[];

    public:
      constexpr size_t u8size() const noexcept;
      constexpr size_t u16size() const noexcept;
      constexpr size_t u32size() const noexcept;
      constexpr size_t u8length() const noexcept;
      constexpr size_t u16length() const noexcept;
      constexpr size_t u32length() const noexcept;
  };

  class u16string : private std::basic_string<char8_t>
  {
    public:
    private:
      char16_t* m_Data;
      size_t    m_Size;
      size_t    m_Capacity;
  };

  class u32string : private std::basic_string<char8_t>
  {
    public:
    private:
      char32_t* m_Data;
      size_t    m_Size;
      size_t    m_Capacity;
  };
} // namespace Cxx
