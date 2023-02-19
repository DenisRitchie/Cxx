#ifndef B5A9594F_915C_489B_ACFA_0EC539CC64F8
#define B5A9594F_915C_489B_ACFA_0EC539CC64F8

#include "Utility.hpp"
#include "Coroutines/Generator.hpp"
#include "DesignPatterns/OutputIterator.hpp"
#include "DesignPatterns/InputIterator.hpp"

#include <algorithm>
#include <compare>
#include <functional>
#include <ranges>
#include <span>
#include <spanstream>
#include <string>
#include <string_view>
#include <numeric>
#include <array>
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
    using CompareThreeWayOrderFallback = Cxx::Details::CustomizationPointObjects::CompareThreeWayOrderFallback;

    class RangeCompare : public NotQuiteObject
    {
      public:
        using NotQuiteObject::NotQuiteObject;

        template <
          std::input_iterator           LeftFirst,
          std::sentinel_for<LeftFirst>  LeftLast,
          std::input_iterator           RightFirst,
          std::sentinel_for<RightFirst> RightLast,
          typename CompareThreeWay = CompareThreeWayOrderFallback,
          typename LeftProjection  = std::identity,
          typename RightProjection = std::identity>
        constexpr auto operator()( //
          LeftFirst&&       left_first,
          LeftLast&&        left_last,
          RightFirst&&      right_first,
          RightLast&&       right_last,
          size_t            number_of_items_to_compare,
          CompareThreeWay&& compare_three_way_order_fallback = {},
          LeftProjection&&  left_projection                  = {},
          RightProjection&& right_projection                 = {}
        ) const
          noexcept(noexcept(std::invoke(
            std::forward<CompareThreeWay>(compare_three_way_order_fallback), //
            /**/ std::invoke(std::forward<LeftProjection>(left_projection), *left_first),
            /**/ std::invoke(std::forward<RightProjection>(right_projection), *right_first)
          ))) //
          -> std::invoke_result_t<CompareThreeWay, projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>>
        {
          using compare_three_way_t = std::invoke_result_t<CompareThreeWay, projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>>;

          // clang-format off
          for ( ; number_of_items_to_compare; --number_of_items_to_compare, left_first = std::ranges::next(left_first), right_first = std::ranges::next(right_first) )
          {
            if ( left_first == left_last ) {
              return right_first == right_last ? compare_three_way_t::equivalent : compare_three_way_t::less;
            }

            if ( right_first == right_last ) {
              return compare_three_way_t::greater;
            }

            const auto compare_result = std::invoke(
              std::forward<CompareThreeWay>(compare_three_way_order_fallback),
                std::invoke(std::forward<LeftProjection>(left_projection), *left_first),
                std::invoke(std::forward<RightProjection>(right_projection), *right_first)
            );

            if ( std::is_neq(compare_result) ) {
              return compare_result;
            }
          }
          // clang-format on

          return compare_three_way_t::equivalent;
        }

        template < //
          std::ranges::input_range LeftRange,
          std::ranges::input_range RightRange,
          typename CompareThreeWay = CompareThreeWayOrderFallback,
          typename LeftProjection  = std::identity,
          typename RightProjection = std::identity>
        constexpr auto operator()( //
          LeftRange&&       left_range,
          RightRange&&      right_range,
          const size_t      number_of_items_to_compare,
          CompareThreeWay&& compare_three_way_order_fallback = {},
          LeftProjection&&  left_projection                  = {},
          RightProjection&& right_projection                 = {}
        ) const noexcept
        {
          return (*this)( //
            std::ranges::begin(std::forward<LeftRange>(left_range)),
            std::ranges::end(std::forward<LeftRange>(left_range)),
            std::ranges::begin(std::forward<RightRange>(right_range)),
            std::ranges::end(std::forward<RightRange>(right_range)),
            number_of_items_to_compare,
            std::forward<CompareThreeWay>(compare_three_way_order_fallback),
            std::forward<LeftProjection>(left_projection),
            std::forward<RightProjection>(right_projection)
          );
        }

        template < //
          std::ranges::input_range LeftRange,
          std::ranges::input_range RightRange,
          typename CompareThreeWay = CompareThreeWayOrderFallback,
          typename LeftProjection  = std::identity,
          typename RightProjection = std::identity>
        constexpr auto operator()( //
          LeftRange&&       left_range,
          RightRange&&      right_range,
          CompareThreeWay&& compare_three_way_order_fallback = {},
          LeftProjection&&  left_projection                  = {},
          RightProjection&& right_projection                 = {}
        ) const noexcept
        {
          return (*this)( //
            std::forward<LeftRange>(left_range),
            std::forward<RightRange>(right_range),
            std::min(std::ranges::size(std::forward<LeftRange>(left_range)), std::ranges::size(std::forward<RightRange>(right_range))),
            std::forward<CompareThreeWay>(compare_three_way_order_fallback),
            std::forward<LeftProjection>(left_projection),
            std::forward<RightProjection>(right_projection)
          );
        }
    };
  } // namespace Details::FunctionObjects

  inline constexpr Details::FunctionObjects::RangeCompare RangeCompare{ NotQuiteObject::ConstructTag{} };

} // namespace Cxx::Algorithms::inline V1

template <std::ranges::contiguous_range Range, std::ranges::forward_range Pattern>
requires std::ranges::view<Range> and std::ranges::view<Pattern> and std::indirectly_comparable<std::ranges::iterator_t<Range>, std::ranges::iterator_t<Pattern>, std::ranges::equal_to>
class std::ranges::split_view<Range, Pattern> : public Cxx::Details::ContiguousSplitView<Range, Pattern>
{
    using Cxx::Details::ContiguousSplitView<Range, Pattern>::ContiguousSplitView;
};

#include "Implementations/Algorithms.tcc"

#endif /* B5A9594F_915C_489B_ACFA_0EC539CC64F8 */
