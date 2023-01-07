#ifndef C1283D43_8489_45EF_845F_3B178A0D300A
#define C1283D43_8489_45EF_845F_3B178A0D300A

#include "IteratorTraits.hpp"

namespace Cxx::Traits
{
  template <typename Container>
  struct ContainerTraits // Representa la interfaz de contenedor
  {
      using value_type      = typename Container::value_type;
      using reference       = typename Container::reference;
      using const_reference = typename Container::const_reference;
      using iterator        = typename Container::iterator;
      using const_iterator  = typename Container::const_iterator;
      using difference_type = typename Container::difference_type;
      using size_type       = typename Container::size_type;
  };
} // namespace Cxx::Traits

namespace Cxx::Concepts
{
  template <typename T>
  concept Container = requires { Traits::ContainerTraits<T>{}; } and std::ranges::range<T>;
} // namespace Cxx::Concepts

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
      using ValueTypeOrDefault                              = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ValueTypeTrait, Container>;
      using ReferenceOrDefault                              = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ReferenceTrait, Container>;
      using ConstReferenceOrDefault                         = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ConstReferenceTrait, Container>;
      using IteratorOrDefault                               = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::IteratorTrait, Container>;
      using SentinelOrDefault                               = DetectedOrType<ArchetypalAlias::DefaultTrait<std::default_sentinel_t>, ArchetypalAlias::SentinelTrait, Container>;
      using ConstIteratorOrDefault                          = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ConstIteratorTrait, Container>;
      using ReverseIteratorOrDefault                        = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ReverseIteratorTrait, Container>;
      using ConstReverseIteratorOrDefault                   = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ConstReverseIteratorTrait, Container>;
      using DifferenceTypeOrDefault                         = DetectedOrType<ArchetypalAlias::DefaultTrait<std::ptrdiff_t>, ArchetypalAlias::DifferenceTypeTrait, Container>;
      using SizeTypeOrDefault                               = DetectedOrType<ArchetypalAlias::DefaultTrait<std::size_t>, ArchetypalAlias::SizeTypeTrait, Container>;

      inline static constexpr bool HasValueType             = IsDetectedV<ArchetypalAlias::ValueTypeTrait, Container>;
      inline static constexpr bool HasReference             = IsDetectedV<ArchetypalAlias::ReferenceTrait, Container>;
      inline static constexpr bool HasConstReference        = IsDetectedV<ArchetypalAlias::ConstReferenceTrait, Container>;
      inline static constexpr bool HasIterator              = IsDetectedV<ArchetypalAlias::IteratorTrait, Container>;
      inline static constexpr bool HasSentinel              = IsDetectedV<ArchetypalAlias::SentinelTrait, Container>;
      inline static constexpr bool HasConstIterator         = IsDetectedV<ArchetypalAlias::ConstIteratorTrait, Container>;
      inline static constexpr bool HasReverseIterator       = IsDetectedV<ArchetypalAlias::ReverseIteratorTrait, Container>;
      inline static constexpr bool HasConstReverseIterator  = IsDetectedV<ArchetypalAlias::ConstReverseIteratorTrait, Container>;
      inline static constexpr bool HasDifferenceType        = IsDetectedV<ArchetypalAlias::DifferenceTypeTrait, Container>;
      inline static constexpr bool HasSizeType              = IsDetectedV<ArchetypalAlias::SizeTypeTrait, Container>;

      inline static constexpr bool HasOutputIterator        = std::ranges::output_range<Container>;
      inline static constexpr bool HasInputIterator         = std::ranges::input_range<Container>;
      inline static constexpr bool HasForwardIterator       = std::ranges::forward_range<Container>;
      inline static constexpr bool HasBidirectionalIterator = std::ranges::bidirectional_range<Container>;
      inline static constexpr bool HasRandomAccessIterator  = std::ranges::random_access_range<Container>;
      inline static constexpr bool HasContiguousIterator    = std::ranges::contiguous_range<Container>;
  };
} // namespace Cxx::DetectionIdiom::Traits

#endif /* C1283D43_8489_45EF_845F_3B178A0D300A */
