#ifndef B5A9594F_915C_489B_ACFA_0EC539CC64F8
#define B5A9594F_915C_489B_ACFA_0EC539CC64F8

#include "Utility.hpp"
#include "Concepts.hpp"
#include "Coroutines/Generator.hpp"
#include "DesignPatterns/OutputIterator.hpp"
#include "DesignPatterns/InputIterator.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <functional>
#include <ranges>
#include <span>
#include <spanstream>
#include <string>
#include <string_view>
#include <numeric>
#include <tuple>
#include <iterator>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <locale>

#include <cstring>
#include <cctype>
#include <cuchar>
#include <ctime>

namespace Cxx::Algorithms::inline V1
{
  /**
   * @brief Convierte un contenedor en una cadena donde cada elemento está separado por un delimitador.
   *
   * @tparam ContainerType El tipo del input_container que contiene todos los elementos a unir en una cadena.
   * @tparam SeparatorType El tipo del separador usado para cada elemento del container.
   * @tparam CharType  El tipo del caracter del objeto string que se usará como retorno.
   * @tparam TraitType El tipo del std::char_trait<CharType> del objeto string que se usará como retorno.
   * @tparam AllocType El tipo del std::allocator<CharType> del objeto string que se usará como retorno.
   *
   * @param[in] container  Referencia del del input_container que contiene los elementos a unir en una cadena.
   * @param[in] separator  Referencia del separador de cada elemento del container.
   *
   * @return Regresa en un string todos los valores del contenedor separados por un delimitador.
   */
  template <typename CharType = char, typename TraitType = std::char_traits<CharType>, typename AllocType = std::allocator<CharType>>
  std::basic_string<CharType, TraitType, AllocType> Join(auto&& container, auto&& separator);

  namespace Details::FunctionObjects
  {
    class RangeCompare : public NotQuiteObject
    {
      private:
        inline static constexpr size_t DefaultSize = static_cast<size_t>(-1);

      public:
        using NotQuiteObject::NotQuiteObject;

        template < // clang-format off
          std::input_iterator          LeftFirst,
          std::sentinel_for<LeftFirst> LeftLast,
          std::input_iterator           RightFirst,
          std::sentinel_for<RightFirst> RightLast,
          std::invocable<std::iter_value_t<LeftFirst>>  LeftProjection,
          std::invocable<std::iter_value_t<RightFirst>> RightProjection,
          std::invocable<projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>> CompareThreeWay>
        constexpr auto operator()(
          LeftFirst&& left_first, LeftLast&& left_last, RightFirst&& right_first, RightLast&& right_last,
          size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection,
          CompareThreeWay&& compare_three_way_order_fallback
        ) const noexcept(noexcept(std::invoke(
            std::forward<CompareThreeWay>(compare_three_way_order_fallback),
              std::invoke(std::forward<LeftProjection>(left_projection), *left_first),
              std::invoke(std::forward<RightProjection>(right_projection), *right_first)
          ))) -> std::invoke_result_t<CompareThreeWay, projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>>;
        // clang-format on

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>, std::ranges::range_value_t<RightRange>> CompareThreeWay>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, LeftProjection&& left_projection, RightProjection&& right_projection) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, LeftProjection>, projected_t<std::ranges::iterator_t<RightRange>, RightProjection>> CompareThreeWay>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, LeftProjection&& left_projection, RightProjection&& right_projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>, std::ranges::range_value_t<RightRange>> CompareThreeWay>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, LeftProjection>, projected_t<std::ranges::iterator_t<RightRange>, RightProjection>> CompareThreeWay>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection>
        requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, Projection&& projection) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, Projection>, projected_t<std::ranges::iterator_t<RightRange>, Projection>> CompareThreeWay>
        requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, Projection&& projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection>
        requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, Projection&& projection) const noexcept;

        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, Projection>, projected_t<std::ranges::iterator_t<RightRange>, Projection>> CompareThreeWay>
        requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, Projection&& projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;
    };
  } // namespace Details::FunctionObjects

  inline constexpr Details::FunctionObjects::RangeCompare RangeCompare{ NotQuiteObject::ConstructTag{} };

  namespace Details::FunctionObjects
  {
    class StrongOrder : public NotQuiteObject
    {
      public:
        using NotQuiteObject::NotQuiteObject;

        template <typename CharType, typename TraitType>
        std::strong_ordering operator()( //
          const std::basic_string_view<CharType, TraitType>                       left_string,
          const std::type_identity_t<std::basic_string_view<CharType, TraitType>> right_string
        ) const noexcept
        {
          return Cxx::Algorithms::RangeCompare(left_string, right_string, std::identity{}, std::strong_order);
        }

        template <typename CharType>
        std::strong_ordering operator()( //
          const ZString<CharType>                       left_string,
          const std::type_identity_t<ZString<CharType>> right_string
        ) const noexcept
        {
          return Cxx::Algorithms::RangeCompare(left_string, right_string, size_t(-1), std::identity{}, std::strong_order);
        }

        template <typename CharType>
        std::strong_ordering operator()( //
          const CharType*                       left_string,
          const std::type_identity_t<CharType>* right_string
        ) const noexcept
        {
          return Cxx::Algorithms::RangeCompare( //
            left_string,
            ZStringSentinel{},
            right_string,
            ZStringSentinel{},
            static_cast<size_t>(-1),
            std::identity{},
            std::identity{},
            std::strong_order
          );
        }

        // template <typename CharType, typename TraitType>
        // std::strong_ordering operator()( //
        //   const CharType*                                   left_string,
        //   const std::basic_string_view<CharType, TraitType> right_string
        // ) const noexcept
        // {
        //   return Cxx::Algorithms::RangeCompare(left_string, ZStringSentinel{}, right_string.data(), ZStringSentinel{}, static_cast<size_t>(-1), std::strong_order);
        // }

        // template <typename CharType, typename TraitType>
        // std::strong_ordering operator()( //
        //   const std::basic_string_view<CharType, TraitType> left_string,
        //   const CharType*                                   right_string
        // ) const noexcept
        // {
        //   return Cxx::Algorithms::RangeCompare(left_string.data(), ZStringSentinel{}, right_string, ZStringSentinel{}, static_cast<size_t>(-1), std::strong_order);
        // }
    };

    class WeakOrder : public NotQuiteObject
    {
      public:
        using NotQuiteObject::NotQuiteObject;
    };

  } // namespace Details::FunctionObjects

  inline constexpr Details::FunctionObjects::StrongOrder strong_order{ NotQuiteObject::ConstructTag{} };

  inline constexpr Details::FunctionObjects::WeakOrder weak_order{ NotQuiteObject::ConstructTag{} };
} // namespace Cxx::Algorithms::inline V1

template <std::ranges::contiguous_range Range, std::ranges::forward_range Pattern>
requires std::ranges::view<Range> and std::ranges::view<Pattern> and std::indirectly_comparable<std::ranges::iterator_t<Range>, std::ranges::iterator_t<Pattern>, std::ranges::equal_to>
class std::ranges::split_view<Range, Pattern> : public Cxx::Details::ContiguousSplitView<Range, Pattern>
{
    using Cxx::Details::ContiguousSplitView<Range, Pattern>::ContiguousSplitView;
};

#include "Implementations/Algorithms.tcc"

#endif /* B5A9594F_915C_489B_ACFA_0EC539CC64F8 */
