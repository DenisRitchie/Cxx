#ifndef CC4CC4EA_6005_4642_BC5F_855D9EC6C211
#define CC4CC4EA_6005_4642_BC5F_855D9EC6C211

#include "TypeTraits.hpp"

#include <utility>
#include <optional>
#include <tuple>
#include <array>
#include <ranges>
#include <string_view>
#include <span>
#include <variant>

namespace Cxx
{
  namespace Utilities
  {
    /**
     * @brief Desreferenciar un puntero de forma recursiva.
     *
     * @tparam T Tipo del puntero a desreferenciar.
     * @param pointer_or_value Puntero a desreferenciar.
     *
     * @return constexpr decltype(auto) Regresa el valor más profundo desreferenciado.
     */
    template <typename T>
    [[nodiscard]] inline constexpr decltype(auto) deep_raw_pointer_dereference(T&& pointer_or_value) noexcept;

    /**
     * @brief Desreferenciar un puntero inteligente de forma recursiva.
     *
     * @tparam T Tipo del puntero inteligente a desreferenciar.
     * @param pointer_or_value Puntero inteligente a desreferenciar.
     *
     * @return constexpr decltype(auto) Regresa el valor más profundo desreferenciado.
     */
    template <typename T>
    [[nodiscard]] inline constexpr decltype(auto) deep_smart_pointer_dereference(T&& pointer_or_value) noexcept;
  } // namespace Utilities

  /**
   * @brief Helper type for the visitor
   *
   * @tparam Ts
   */
  template <class... Ts>
  struct Overloaded : Ts...
  {
      using Ts::operator()...;
  };

  /**
   * @brief Explicit deduction guide (not needed as of C++20)
   *
   * @tparam Ts
   */
  template <class... Ts>
  Overloaded(Ts...) -> Overloaded<Ts...>;

  struct CompareThreeWayOrderFallback
  {
      template <typename LeftType, typename RightType>
      inline constexpr auto operator()(LeftType&& left, RightType&& right) const -> Traits::common_comparison_category_t<LeftType, RightType>
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
  };

  template <typename LeftType, typename RightType>
  inline constexpr auto Compare3WayOrderFallback(LeftType&& left, RightType&& right)
  {
    return CompareThreeWayOrderFallback{}(std::forward<LeftType>(left), std::forward<RightType>(right));
  }

  /**
   * @brief Clase usada como std::end(object) para las cadenas terminadas en Cero.
   *
   *  ZStringSentinel representa la marca de fin de texto para las cadenas terminadas en Cero.
   *  El final del texto es representado por el caracter '\0'.
   *
   *  Por ejemplo: std::begin("Texto") != ZStringSentinel{ }
   */
  struct ZStringSentinel
  {
      // clang-format off
      constexpr bool operator==(const      char* pointer) const noexcept;
      constexpr bool operator==(const    int8_t* pointer) const noexcept;
      constexpr bool operator==(const   uint8_t* pointer) const noexcept;
      constexpr bool operator==(const  uint16_t* pointer) const noexcept;
      constexpr bool operator==(const  uint32_t* pointer) const noexcept;
      constexpr bool operator==(const   wchar_t* pointer) const noexcept;
      constexpr bool operator==(const   char8_t* pointer) const noexcept;
      constexpr bool operator==(const  char16_t* pointer) const noexcept;
      constexpr bool operator==(const  char32_t* pointer) const noexcept;
      constexpr bool operator==(const std::byte* pointer) const noexcept;
      // clang-format on
  };

  /**
   * @brief Clase usada como std::ranges::view para las cadenas terminadas en Cero.
   *
   * @tparam CharType Tipo del caracter de la cadena terminada en Cero.
   * @todo   Implementar todos los métodos std::contiguous_iterator que espera el tipo base std::ranges::view_interface
   */
  template <typename CharType>
  requires Cxx::Traits::IsAnyOf<CharType, char, int8_t, uint8_t, uint16_t, uint32_t, wchar_t, char8_t, char16_t, char32_t, std::byte>
  class ZString : std::ranges::view_interface<ZString<CharType>>
  {
    public:
      using value_type      = CharType;
      using pointer         = value_type*;
      using const_pointer   = const value_type*;
      using reference       = value_type&;
      using const_reference = const value_type&;
      using iterator        = value_type*;
      using const_iterator  = const value_type*;
      using difference_type = std::ptrdiff_t;
      using size_type       = std::size_t;

      constexpr ZString(const_pointer pointer) noexcept
        : m_Pointer{ pointer }
      {
      }

      template <size_t Size>
      constexpr ZString(const value_type (&ref_array)[Size]) noexcept
        : m_Pointer{ ref_array }
      {
      }

      template <typename TraitType>
      constexpr ZString(const std::basic_string_view<CharType, TraitType>& string_view) noexcept
        : m_Pointer{ string_view.data() }
      {
      }

      template <size_t Size>
      constexpr ZString(const std::array<CharType, Size>& array) noexcept
        : m_Pointer{ array.data() }
      {
      }

      template <size_t Size>
      constexpr ZString(const std::span<CharType, Size>& span) noexcept
        : m_Pointer{ span.data() }
      {
      }

      constexpr const_iterator begin() const noexcept
      {
        return m_Pointer;
      }

      constexpr ZStringSentinel end() const noexcept
      {
        return {};
      }

    private:
      const_pointer m_Pointer{ nullptr };
  };

  namespace Details
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
  } // namespace Details
} // namespace Cxx

#include "Implementations/Utility.tcc"

#endif /* CC4CC4EA_6005_4642_BC5F_855D9EC6C211 */
