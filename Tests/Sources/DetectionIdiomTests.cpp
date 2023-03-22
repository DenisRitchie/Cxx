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

namespace std
{
  // DetectionIdiomTests - ContainerTraits

  template <typename Container>
  bool operator==(const back_insert_iterator<Container>&, const back_insert_iterator<Container>&)
  {
    return true;
  }

  // DetectionIdiomTests - ContainerTraits

  template <typename Container>
  bool operator==(const back_insert_iterator<Container>&, default_sentinel_t)
  {
    return true;
  }

  // DetectionIdiomTests - ContainerTraits

  template <typename Container>
  bool operator==(default_sentinel_t, const back_insert_iterator<Container>&)
  {
    return true;
  }
} // namespace std

TEST(DetectionIdiomTests, ContainerTraits)
{
  EXPECT_FALSE((Cxx::Concepts::Container<int32_t>));
  EXPECT_FALSE((Cxx::Concepts::Container<int32_t*>));
  EXPECT_TRUE((Cxx::Concepts::Container<std::string>));
  EXPECT_TRUE((Cxx::Concepts::Container<std::string_view>));
  EXPECT_TRUE((Cxx::Concepts::Container<std::deque<int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::Container<std::list<int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::Container<std::forward_list<int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::Container<std::map<int32_t, int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::Container<std::array<int32_t, 1>>));
  EXPECT_TRUE((Cxx::Concepts::OutputContainer<std::vector<int32_t>, int32_t>));
  EXPECT_TRUE((Cxx::Concepts::InputContainer<std::vector<int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::ForwardContainer<std::vector<int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::BidirectionalContainer<std::vector<int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::RandomAccessContainer<std::vector<int32_t>>));
  EXPECT_TRUE((Cxx::Concepts::ContiguousContainer<std::vector<int32_t>>));

  struct OutputContainer
  {
      using value_type      = Cxx::Traits::ContainerTraits<std::vector<int32_t>>::value_type;
      using reference       = Cxx::Traits::ContainerTraits<std::vector<int32_t>>::reference;
      using const_reference = Cxx::Traits::ContainerTraits<std::vector<int32_t>>::const_reference;
      using difference_type = Cxx::Traits::ContainerTraits<std::vector<int32_t>>::difference_type;
      using size_type       = Cxx::Traits::ContainerTraits<std::vector<int32_t>>::size_type;
      using iterator        = std::back_insert_iterator<std::vector<int32_t>>;
      using const_iterator  = std::back_insert_iterator<std::vector<int32_t>>;

      iterator begin()
      {
        return std::back_inserter(Values);
      }

      std::default_sentinel_t end()
      {
        return std::default_sentinel;
      }

      std::vector<int32_t> Values;
  };

  EXPECT_TRUE((Cxx::Concepts::Container<OutputContainer>));
  EXPECT_TRUE((Cxx::Concepts::OutputContainer<OutputContainer, int32_t>));
  EXPECT_FALSE((Cxx::Concepts::InputContainer<OutputContainer>));
  EXPECT_FALSE((Cxx::Concepts::ForwardContainer<OutputContainer>));
  EXPECT_FALSE((Cxx::Concepts::BidirectionalContainer<OutputContainer>));
  EXPECT_FALSE((Cxx::Concepts::RandomAccessContainer<OutputContainer>));
  EXPECT_FALSE((Cxx::Concepts::ContiguousContainer<OutputContainer>));
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
  EXPECT_TRUE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::string>>));
  EXPECT_TRUE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::string_view>>));
  EXPECT_TRUE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::vector<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::deque<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::forward_list<int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::map<int32_t, int32_t>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<std::back_insert_iterator<std::vector<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::Cpp20Iterator<std::ranges::iterator_t<std::array<int32_t, 1>>>));
  EXPECT_FALSE((Cxx::Concepts::Cpp20Iterator<int32_t>));
  EXPECT_TRUE((Cxx::Concepts::BidirectionalIterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::ForwardIterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::InputIterator<std::ranges::iterator_t<std::list<int32_t>>>));
  EXPECT_TRUE((Cxx::Concepts::OutputIterator<std::ranges::iterator_t<std::list<int32_t>>, int32_t>));
  EXPECT_FALSE((Cxx::Concepts::OutputIterator<int32_t, int32_t>));
  EXPECT_TRUE((Cxx::Concepts::OutputIterator<std::back_insert_iterator<std::vector<int32_t>>, int32_t>));
}
