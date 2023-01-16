#ifndef B5A9594F_915C_489B_ACFA_0EC539CC64F8
#define B5A9594F_915C_489B_ACFA_0EC539CC64F8

#include "Utility.hpp"
#include "Coroutines/Generator.hpp"
#include "DesignPatterns/OutputIterator.hpp"
#include "DesignPatterns/InputIterator.hpp"

#include <cstring>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <ranges>

namespace Cxx::inline V1
{
  template <typename CharType = char, typename TraitType = std::char_traits<CharType>, typename AllocType = std::allocator<CharType>>
  std::basic_string<CharType, TraitType, AllocType> Join(auto&& container, auto&& separator) // clang-format off
  requires requires(decltype(container) range)
  {
    { std::begin(range) } -> std::input_iterator;
    { std::end(range)   } -> std::input_iterator;
  } //clang-format on
  {
    using ContainerType = decltype(container);
    using SeparatorType = decltype(separator);

    std::basic_ostringstream<CharType, TraitType, AllocType> output;

    std::ranges::copy( //
      std::begin(std::forward<ContainerType>(container)),
      std::end(std::forward<ContainerType>(container)),
      Cxx::DesignPatterns::MakeOstreamJoiner(output, std::forward<SeparatorType>(separator))
    );

    return output.str();
  };
} // namespace Cxx::inline V1

#endif /* B5A9594F_915C_489B_ACFA_0EC539CC64F8 */
