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

#include <cstring>
#include <cctype>
#include <cuchar>
#include <ctime>

namespace Cxx::inline V1
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

  namespace Algorithms
  {
    namespace Ranges
    {
      namespace Details
      {
      } // namespace Details

      struct ContiguousCompare
      {
          template <std::contiguous_iterator LeftFirst, std::sentinel_for<LeftFirst> LeftLast, std::contiguous_iterator RightFirst, std::sentinel_for<RightFirst> RightLast, typename LeftProjection = std::identity, typename RightProjection = std::identity>
          constexpr auto operator()(LeftFirst&& left_first, LeftLast&& left_last, RightFirst&& right_first, RightLast&& right_last, size_t length, LeftProjection&& left_projection = {}, RightProjection&& right_projection = {}) const
            -> Traits::common_comparison_category_t<std::invoke_result_t<LeftProjection, std::iter_value_t<LeftFirst>>, std::invoke_result_t<RightProjection, std::iter_value_t<RightFirst>>>
          {
            while ( length-- )
            {
              if ( (left_first == left_last) or (right_first == right_last) )
                break;

              auto&& projected_value1 = std::invoke(left_projection, *left_first++);
              auto&& projected_value2 = std::invoke(right_projection, *right_first++);

              if ( auto compare_result = Compare3WayOrderFallback(projected_value1, projected_value2); compare_result != 0 )
                return compare_result;
            }

            return Traits::common_comparison_category_t<std::invoke_result_t<LeftProjection, std::iter_value_t<LeftFirst>>, std::invoke_result_t<RightProjection, std::iter_value_t<RightFirst>>>::equivalent;
          }

          template <std::ranges::contiguous_range LeftRange, std::ranges::contiguous_range RightRange, typename LeftProjection = std::identity, typename RightProjection = std::identity>
          constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t length, LeftProjection&& left_projection = {}, RightProjection&& right_projection = {}) const
          {
            return (*this)( //
              std::ranges::begin(std::forward<LeftRange>(left_range)),
              std::ranges::end(std::forward<LeftRange>(left_range)),
              std::ranges::begin(std::forward<RightRange>(right_range)),
              std::ranges::end(std::forward<RightRange>(right_range)),
              length,
              std::forward<LeftProjection>(left_projection),
              std::forward<RightProjection>(right_projection)
            );
          }

          template <std::ranges::contiguous_range LeftRange, std::ranges::contiguous_range RightRange, typename LeftProjection = std::identity, typename RightProjection = std::identity>
          constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, LeftProjection&& left_projection = {}, RightProjection&& right_projection = {}) const
          {
            return (*this)( //
              std::forward<LeftRange>(left_range),
              std::forward<RightRange>(right_range),
              std::min( //
                std::ranges::size(std::forward<LeftRange>(left_range)),
                std::ranges::size(std::forward<RightRange>(right_range))
              ),
              std::forward<LeftProjection>(left_projection),
              std::forward<RightProjection>(right_projection)
            );
          }
      };
    } // namespace Ranges
  }   // namespace Algorithms

  inline constexpr Algorithms::Ranges::ContiguousCompare RangeCompare;

  inline namespace CustomizationPointObjects
  {
  } // namespace CustomizationPointObjects
} // namespace Cxx::inline V1

template <std::ranges::contiguous_range Range, std::ranges::forward_range Pattern>
requires std::ranges::view<Range> and std::ranges::view<Pattern> and std::indirectly_comparable<std::ranges::iterator_t<Range>, std::ranges::iterator_t<Pattern>, std::ranges::equal_to>
class std::ranges::split_view<Range, Pattern> : public Cxx::Details::ContiguousSplitView<Range, Pattern>
{
    using Cxx::Details::ContiguousSplitView<Range, Pattern>::ContiguousSplitView;
};

#include "Implementations/Algorithms.tcc"

#endif /* B5A9594F_915C_489B_ACFA_0EC539CC64F8 */
