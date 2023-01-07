#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Cxx/DetectionIdiom.hpp"
#include "Cxx/ContainerTraits.hpp"
#include "Cxx/IteratorTraits.hpp"

#include <iostream>
#include <string_view>
#include <string>
#include <iterator>
#include <vector>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <tuple>

namespace ArchetypalAlias
{
  template <typename TContainer>
  using ContainerBegin = decltype(std::declval<TContainer>().begin());

  template <typename TContainer>
  using ContainerEnd = decltype(std::declval<TContainer>().end());

  template <typename TContainer>
  using ContainerSize = decltype(std::declval<TContainer>().size());

  template <typename TContainer>
  using ContainerValueType = typename TContainer::value_type;

} // namespace ArchetypalAlias

TEST(DetectionIdiomTests, DetectionIdiom)
{
  EXPECT_TRUE((Cxx::DetectionIdiom::IsDetectedV<ArchetypalAlias::ContainerBegin, std::vector<int32_t>>));
  EXPECT_TRUE((Cxx::DetectionIdiom::IsDetectedV<ArchetypalAlias::ContainerEnd, std::vector<int32_t>>));
  EXPECT_TRUE((Cxx::DetectionIdiom::IsDetectedV<ArchetypalAlias::ContainerSize, std::vector<int32_t>>));
  EXPECT_TRUE((Cxx::DetectionIdiom::IsDetectedV<ArchetypalAlias::ContainerValueType, std::vector<int32_t>>));
  EXPECT_FALSE((Cxx::DetectionIdiom::IsDetectedV<ArchetypalAlias::ContainerBegin, std::iostream>));
  EXPECT_TRUE((Cxx::DetectionIdiom::IsDetectedExactV<char32_t, ArchetypalAlias::ContainerValueType, std::u32string>));
  EXPECT_TRUE((std::same_as<Cxx::DetectionIdiom::DetectedType<ArchetypalAlias::ContainerValueType, std::u8string>, char8_t>));
  EXPECT_TRUE((std::same_as<Cxx::DetectionIdiom::DetectedOrType<Cxx::DetectionIdiom::Nonesuch, ArchetypalAlias::ContainerEnd, int32_t>, Cxx::DetectionIdiom::Nonesuch>));
  EXPECT_TRUE((Cxx::DetectionIdiom::IsDetectedConvertibleV<char32_t, ArchetypalAlias::ContainerValueType, std::string_view>));
}

TEST(DetectionIdiomTests, ContainerTraits)
{
}

TEST(DetectionIdiomTests, IteratorTraits)
{
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::ranges::iterator_t<std::string>>));
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::ranges::iterator_t<std::string_view>>));
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::ranges::iterator_t<std::vector<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::ranges::iterator_t<std::deque<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::ranges::iterator_t<std::forward_list<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::ranges::iterator_t<std::map<int32_t, int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::back_insert_iterator<std::vector<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::Iterator<std::ranges::iterator_t<std::array<int32_t, 1>>>));
  EXPECT_FALSE((Cxx::Concepts::Iterator<int32_t>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::string>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::string_view>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::vector<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::deque<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::forward_list<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::map<int32_t, int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::back_insert_iterator<std::vector<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::array<int32_t, 1>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<int32_t>));
  EXPECT_TRUE((Cxx::Concepts::BidirectionalIterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::ForwardIterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::InputIterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::OutputIterator<std::ranges::iterator_t<std::list<int32_t>>, int32_t>));
  EXPECT_FALSE((Cxx::Concepts::OutputIterator<int32_t, int32_t>));
  EXPECT_TRUE((Cxx::Concepts::OutputIterator<std::back_insert_iterator<std::vector<int32_t>>, int32_t>));
}
