#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <algorithm>
#include <charconv>
#include <chrono>
#include <concepts>
#include <future>
#include <iostream>
#include <iomanip>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <span>
#include <spanstream>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std::string_literals;
using namespace std::string_view_literals;

template <typename T>
void TypeIdentityFunction([[maybe_unused]] T value1, [[maybe_unused]] std::type_identity_t<T> value2)
{
}

template <typename CharType, typename TraitType>
void Compare( //
  [[maybe_unused]] const std::basic_string_view<CharType, TraitType>                       string1,
  [[maybe_unused]] const std::type_identity_t<std::basic_string_view<CharType, TraitType>> string2
)
{
}

// template <class _Elem, class _Traits, int = 2> // TRANSITION, VSO-409326
// _NODISCARD constexpr bool operator==(const basic_string_view<_Elem, _Traits> _Lhs, const _Identity_t<basic_string_view<_Elem, _Traits>> _Rhs) noexcept
// {
//   return _Lhs._Equal(_Rhs);
// }

TEST(StandardLibrary, TypeIdentity)
{
  // Compare("Texto", "Otro Texto");
  //"Texto"sv == "Otro Texto";
}

TEST(StandardLibrary, LexicographicalCompare)
{
  // indirect_strict_weak_order<projected<_It1, _Pj1>, projected<_It2, _Pj2>>
  // std::indirect_strict_weak_order<>
}
