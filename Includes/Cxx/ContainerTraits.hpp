#ifndef C1283D43_8489_45EF_845F_3B178A0D300A
#define C1283D43_8489_45EF_845F_3B178A0D300A

#include "IteratorTraits.hpp"

namespace Cxx::DetectionIdiom::Traits
{
  namespace ArchetypalAlias
  {
    template <typename T>
    using IteratorTrait = std::ranges::iterator_t<std::remove_cvref_t<T>>;

    template <typename T>
    using SentinelTrait = std::ranges::sentinel_t<std::remove_cvref_t<T>>;

    template <typename T>
    using ConstIteratorTrait = typename std::remove_cvref_t<T>::const_iterator;

    template <typename T>
    using ReverseIteratorTrait = typename std::remove_cvref_t<T>::reverse_iterator;

    template <typename T>
    using ConstReverseIteratorTrait = typename std::remove_cvref_t<T>::const_reverse_iterator;

    template <typename T>
    using SizeTypeTrait = typename std::remove_cvref_t<T>::size_type;
  }; // namespace ArchetypalAlias

  template <typename Container>
  struct ContainerTraits
  {
      // clang-format off
      using ValueTypeOrDefault            = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ValueTypeTrait, Container>;
      using ReferenceOrDefault            = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ReferenceTrait, Container>;
      using ConstReferenceOrDefault       = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ConstReferenceTrait, Container>;
      using IteratorOrDefault             = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::IteratorTrait, Container>;
      using SentinelOrDefault             = DetectedOrType<ArchetypalAlias::DefaultTrait<std::default_sentinel_t>, ArchetypalAlias::SentinelTrait, Container>;
      using ConstIteratorOrDefault        = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ConstIteratorTrait, Container>;
      using ReverseIteratorOrDefault      = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ReverseIteratorTrait, Container>;
      using ConstReverseIteratorOrDefault = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ConstReverseIteratorTrait, Container>;
      using DifferenceTypeOrDefault       = DetectedOrType<ArchetypalAlias::DefaultTrait<std::ptrdiff_t>, ArchetypalAlias::DifferenceTypeTrait, Container>;
      using SizeTypeOrDefault             = DetectedOrType<ArchetypalAlias::DefaultTrait<std::size_t>, ArchetypalAlias::SizeTypeTrait, Container>;
      // clang-format on

      inline static constexpr bool HasValueType            = IsDetectedV<ArchetypalAlias::ValueTypeTrait, Container>;
      inline static constexpr bool HasReference            = IsDetectedV<ArchetypalAlias::ReferenceTrait, Container>;
      inline static constexpr bool HasConstReference       = IsDetectedV<ArchetypalAlias::ConstReferenceTrait, Container>;
      inline static constexpr bool HasIterator             = IsDetectedV<ArchetypalAlias::IteratorTrait, Container>;
      inline static constexpr bool HasSentinel             = IsDetectedV<ArchetypalAlias::SentinelTrait, Container>;
      inline static constexpr bool HasConstIterator        = IsDetectedV<ArchetypalAlias::ConstIteratorTrait, Container>;
      inline static constexpr bool HasReverseIterator      = IsDetectedV<ArchetypalAlias::ReverseIteratorTrait, Container>;
      inline static constexpr bool HasConstReverseIterator = IsDetectedV<ArchetypalAlias::ConstReverseIteratorTrait, Container>;
      inline static constexpr bool HasDifferenceType       = IsDetectedV<ArchetypalAlias::DifferenceTypeTrait, Container>;
      inline static constexpr bool HasSizeType             = IsDetectedV<ArchetypalAlias::SizeTypeTrait, Container>;

      template <typename ValueType>
      inline static constexpr bool HasOutputIterator        = std::ranges::output_range<Container, ValueType>;
      inline static constexpr bool HasInputIterator         = std::ranges::input_range<Container>;
      inline static constexpr bool HasForwardIterator       = std::ranges::forward_range<Container>;
      inline static constexpr bool HasBidirectionalIterator = std::ranges::bidirectional_range<Container>;
      inline static constexpr bool HasRandomAccessIterator  = std::ranges::random_access_range<Container>;
      inline static constexpr bool HasContiguousIterator    = std::ranges::contiguous_range<Container>;
  };
} // namespace Cxx::DetectionIdiom::Traits

namespace Cxx::Traits
{
  template <typename Container>
  struct ContainerTraits
  {
      using value_type      = DetectionIdiom::Traits::ContainerTraits<Container>::ValueTypeOrDefault;      // typename Container::value_type
      using reference       = DetectionIdiom::Traits::ContainerTraits<Container>::ReferenceOrDefault;      // typename Container::reference
      using const_reference = DetectionIdiom::Traits::ContainerTraits<Container>::ConstReferenceOrDefault; // typename Container::const_reference
      using iterator        = DetectionIdiom::Traits::ContainerTraits<Container>::IteratorOrDefault;       // typename Container::iterator
      using const_iterator  = DetectionIdiom::Traits::ContainerTraits<Container>::ConstIteratorOrDefault;  // typename Container::const_iterator
      using difference_type = DetectionIdiom::Traits::ContainerTraits<Container>::DifferenceTypeOrDefault; // typename Container::difference_type
      using size_type       = DetectionIdiom::Traits::ContainerTraits<Container>::SizeTypeOrDefault;       // typename Container::size_type
  };
} // namespace Cxx::Traits

namespace Cxx::Concepts
{
  // clang-format off
  template <typename T>
  concept Container = std::ranges::range<T> and requires
  {
    typename T::value_type;
    typename T::reference;
    typename T::const_reference;
    typename T::iterator;
    typename T::const_iterator;
    typename T::difference_type;
    typename T::size_type;
  };
  // clang-format on

  template <typename C, typename T>
  concept OutputContainer = Container<C> and std::ranges::output_range<C, T>;

  template <typename C>
  concept InputContainer = Container<C> and std::ranges::input_range<C>;

  template <typename C>
  concept ForwardContainer = Container<C> and std::ranges::forward_range<C>;

  template <typename C>
  concept BidirectionalContainer = Container<C> and std::ranges::bidirectional_range<C>;

  template <typename C>
  concept RandomAccessContainer = Container<C> and std::ranges::random_access_range<C>;

  template <typename C>
  concept ContiguousContainer = Container<C> and std::ranges::contiguous_range<C>;
} // namespace Cxx::Concepts

#endif /* C1283D43_8489_45EF_845F_3B178A0D300A */
