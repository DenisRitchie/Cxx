#ifndef A950AF88_4197_4027_833F_B206920F0EE5
#define A950AF88_4197_4027_833F_B206920F0EE5

#include "Cxx/FunctionTraits.hpp"
#include "Cxx/ContainerTraits.hpp"

#include <ranges>
#include <algorithm>
#include <string>
#include <string_view>
#include <iostream>
#include <optional>
#include <span>

namespace Cxx::Details
{
  //
  // https://brevzin.github.io/c++/2020/07/06/split-view/
  //

  template <std::ranges::contiguous_range Range, std::ranges::forward_range Pattern>
  requires std::ranges::view<Range> and std::ranges::view<Pattern> and std::indirectly_comparable<std::ranges::iterator_t<Range>, std::ranges::iterator_t<Pattern>, std::ranges::equal_to>
  class ContiguousSplitView : public std::ranges::view_interface<ContiguousSplitView<Range, Pattern>>
  {
    public:
      ContiguousSplitView() = default;

      ContiguousSplitView(Range&& range, Pattern&& pattern)
        : m_Range{ std::move(range) }
        , m_Pattern{ std::move(pattern) }
      {
      }

      template <std::ranges::contiguous_range Container>
      requires std::constructible_from<Range, std::ranges::views::all_t<Container>>
      ContiguousSplitView(Container&& range, std::ranges::range_value_t<Container>&& pattern)
        : m_Range{ std::ranges::views::all(std::forward<Container>(range)) }
        , m_Pattern{ std::move(pattern) }
      {
      }

      class Iterator
      {
        public:
          friend struct Sentinel;

          using underlying_t = std::remove_reference_t<std::ranges::range_reference_t<Range>>;

          struct value_type : std::span<underlying_t>
          {
              using std::span<underlying_t>::span;

              operator std::basic_string_view<std::ranges::range_value_t<Range>>() const noexcept
              requires Cxx::Traits::IsAnyOf<std::ranges::range_value_t<Range>, char, int8_t, uint8_t, uint16_t, uint32_t, wchar_t, char8_t, char16_t, char32_t>
              {
                return { this->data(), this->size() };
              }
          };

          using iterator_category = std::forward_iterator_tag;
          using iterator_concept  = std::forward_iterator_tag;
          using pointer           = value_type*;
          using const_pointer     = const value_type*;
          using reference         = value_type&;
          using const_reference   = const value_type&;
          using difference_type   = std::ptrdiff_t;

          Iterator() = default;

          Iterator(ContiguousSplitView* parent)
            : m_Parent{ parent }
            , m_Current{ std::ranges::begin(parent->m_Range) }
            , m_Next{ LookupNext() }
          {
          }

          Iterator(std::default_sentinel_t, ContiguousSplitView* parent)
            : m_Parent{ parent }
            , m_Current{ std::ranges::end(parent->m_Range) }
            , m_Next{}
          {
          }

          bool operator==(const Iterator& right) const noexcept
          {
            return m_Current == right.m_Current;
          }

          auto operator++() -> Iterator&
          {
            if ( m_Next != std::ranges::end(m_Parent->m_Range) )
            {
              m_Current = std::ranges::next(m_Next);
              m_Next    = LookupNext();
            }
            else
            {
              m_Current = m_Next;
            }

            return *this;
          }

          auto operator++(int32_t) -> Iterator
          {
            auto temporary = *this;
            ++*this;
            return temporary;
          }

          auto operator*() const -> value_type
          {
            return { m_Current, m_Next };
          }

        private:
          auto LookupNext() const -> std::ranges::iterator_t<Range>
          {
            auto subrange = std::ranges::search(std::ranges::subrange(m_Current, std::ranges::end(m_Parent->m_Range)), m_Parent->m_Pattern);
            return subrange.begin();
          }

          ContiguousSplitView*           m_Parent{ nullptr };
          std::ranges::iterator_t<Range> m_Current;
          std::ranges::iterator_t<Range> m_Next;
      };

      struct Sentinel
      {
          bool operator==(const Iterator& right) const noexcept
          {
            return right.m_Current == m_Sentinel;
          }

          std::ranges::sentinel_t<Range> m_Sentinel;
      };

      auto begin() -> Iterator
      {
        if ( not m_CachedBegin )
        {
          m_CachedBegin.emplace(this);
        }

        return *m_CachedBegin;
      }

      auto end() -> Sentinel
      {
        return { std::ranges::end(m_Range) };
      }

      auto end() -> Iterator
      requires std::ranges::common_range<Range>
      {
        return { std::default_sentinel, this };
      }

    private:
      Range                   m_Range;
      Pattern                 m_Pattern;
      std::optional<Iterator> m_CachedBegin;
  };
} // namespace Cxx::Details

template <std::ranges::contiguous_range Range, std::ranges::forward_range Pattern>
requires std::ranges::view<Range> and std::ranges::view<Pattern> and std::indirectly_comparable<std::ranges::iterator_t<Range>, std::ranges::iterator_t<Pattern>, std::ranges::equal_to>
class std::ranges::split_view<Range, Pattern> : public Cxx::Details::ContiguousSplitView<Range, Pattern>
{
    using Cxx::Details::ContiguousSplitView<Range, Pattern>::ContiguousSplitView;
};

namespace Cxx
{
  struct ZStringSentinel
  {
      constexpr bool operator==(const char* pointer) const noexcept
      {
        return *pointer == '\0';
      }

      constexpr bool operator==(const int8_t* pointer) const noexcept
      {
        return *pointer == int8_t{ '\0' };
      }

      constexpr bool operator==(const uint8_t* pointer) const noexcept
      {
        return *pointer == uint8_t{ '\0' };
      }

      constexpr bool operator==(const uint16_t* pointer) const noexcept
      {
        return *pointer == uint16_t{ '\0' };
      }

      constexpr bool operator==(const uint32_t* pointer) const noexcept
      {
        return *pointer == uint32_t{ '\0' };
      }

      constexpr bool operator==(const wchar_t* pointer) const noexcept
      {
        return *pointer == L'\0';
      }

      constexpr bool operator==(const char8_t* pointer) const noexcept
      {
        return *pointer == u8'\0';
      }

      constexpr bool operator==(const char16_t* pointer) const noexcept
      {
        return *pointer == u'\0';
      }

      constexpr bool operator==(const char32_t* pointer) const noexcept
      {
        return *pointer == U'\0';
      }

      constexpr bool operator==(const std::byte* pointer) const noexcept
      {
        return *pointer == std::byte{ '\0' };
      }
  };

  template <typename CharType>
  requires Cxx::Traits::IsAnyOf<CharType, char, int8_t, uint8_t, uint16_t, uint32_t, wchar_t, char8_t, char16_t, char32_t, std::byte>
  struct ZString : std::ranges::view_interface<ZString<CharType>>
  {
      using value_type      = CharType;
      using reference       = value_type&;
      using const_reference = const value_type&;
      using iterator        = value_type*;
      using const_iterator  = const value_type*;
      using difference_type = std::ptrdiff_t;
      using size_type       = std::size_t;

      const CharType* Pointer = nullptr;

      constexpr ZString() = default;

      constexpr ZString(const CharType* pointer) noexcept
        : Pointer{ pointer }
      {
      }

      constexpr auto begin() const noexcept
      {
        return Pointer;
      }

      constexpr auto end() const noexcept
      {
        return ZStringSentinel{};
      }
  };
} // namespace Cxx

#endif /* A950AF88_4197_4027_833F_B206920F0EE5 */
