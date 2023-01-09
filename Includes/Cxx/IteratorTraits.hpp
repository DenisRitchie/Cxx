// https://en.cppreference.com/w/cpp/iterator/iterator_tags

#ifndef A2427B5C_8C4F_4F4F_B256_CA7A977DA926
#define A2427B5C_8C4F_4F4F_B256_CA7A977DA926

#include "DetectionIdiom.hpp"

namespace Cxx::DetectionIdiom::Traits
{
  namespace ArchetypalAlias
  {
    template <typename T>
    using DefaultTrait = T;

    template <typename T>
    using ValueTypeTrait = typename std::remove_cvref_t<T>::value_type;

    template <typename T>
    using ContainerTypeTrait = typename std::remove_cvref_t<T>::container_type;

    template <typename T>
    using IteratorCategoryTrait = typename std::remove_cvref_t<T>::iterator_category;

    template <typename T>
    using IteratorConceptTrait = typename std::remove_cvref_t<T>::iterator_concept;

    template <typename T>
    using PointerTrait = typename std::remove_cvref_t<T>::pointer;

    template <typename T>
    using ConstPointerTrait = typename std::remove_cvref_t<T>::const_pointer;

    template <typename T>
    using ReferenceTrait = typename std::remove_cvref_t<T>::reference;

    template <typename T>
    using ConstReferenceTrait = typename std::remove_cvref_t<T>::const_reference;

    template <typename T>
    using DifferenceTypeTrait = typename std::remove_cvref_t<T>::difference_type;
  }; // namespace ArchetypalAlias

  namespace Details
  {
    // clang-format off
    template <typename Iterator>
    using DefaultIteratorTag =
      std::conditional_t<std::contiguous_iterator<Iterator>,
          std::contiguous_iterator_tag,
          std::conditional_t<std::random_access_iterator<Iterator>,
              std::random_access_iterator_tag,
              std::conditional_t<std::bidirectional_iterator<Iterator>,
                  std::bidirectional_iterator_tag,
                  std::conditional_t<std::forward_iterator<Iterator>,
                      std::forward_iterator_tag,
                      std::conditional_t<std::input_iterator<Iterator>,
                          std::input_iterator_tag,
                          std::conditional_t<IsDetectedExactV<std::output_iterator_tag, DetectedOrType<ArchetypalAlias::DefaultTrait<DetectedOrType<ArchetypalAlias::DefaultTrait<std::input_iterator_tag>, ArchetypalAlias::IteratorCategoryTrait, Iterator>>, ArchetypalAlias::IteratorConceptTrait, Iterator>, Iterator>,
                            std::output_iterator_tag, std::input_iterator_tag
                          > // std::conditional_t<IsDetectedExactV<std::output_iterator_tag, ...>
                      > // std::conditional_t<std::input_iterator<Iterator>, ...>
                  > // std::conditional_t<std::forward_iterator<Iterator>, ...>
              > // std::conditional_t<std::bidirectional_iterator<Iterator>, ...>
          > // std::conditional_t<std::random_access_iterator<Iterator>, ...>
      >; // std::conditional_t<std::contiguous_iterator<Iterator>, ...>
    // clang-format on
  } // namespace Details

  template <typename Iterator>
  struct IteratorTraits
  {
      using ValueTypeOrDefault        = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ValueTypeTrait, Iterator>;
      using ContainerTypeOrDefault    = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ContainerTypeTrait, Iterator>;
      using IteratorCategoryOrDefault = DetectedOrType<ArchetypalAlias::DefaultTrait<Details::DefaultIteratorTag<Iterator>>, ArchetypalAlias::IteratorCategoryTrait, Iterator>;
      using IteratorConceptOrDefault  = DetectedOrType<ArchetypalAlias::DefaultTrait<Details::DefaultIteratorTag<Iterator>>, ArchetypalAlias::IteratorConceptTrait, Iterator>;
      using PointerOrDefault          = DetectedOrType<ArchetypalAlias::DefaultTrait<std::add_pointer_t<ValueTypeOrDefault>>, ArchetypalAlias::PointerTrait, Iterator>;
      using ConstPointerOrDefault     = DetectedOrType<ArchetypalAlias::DefaultTrait<std::add_pointer_t<std::add_const_t<ValueTypeOrDefault>>>, ArchetypalAlias::ConstPointerTrait, Iterator>;
      using ReferenceOrDefault        = DetectedOrType<ArchetypalAlias::DefaultTrait<std::add_lvalue_reference_t<ValueTypeOrDefault>>, ArchetypalAlias::ReferenceTrait, Iterator>;
      using ConstReferenceOrDefault   = DetectedOrType<ArchetypalAlias::DefaultTrait<std::conditional_t<std::is_void_v<ReferenceOrDefault>, void, std::add_lvalue_reference_t<std::add_const_t<ValueTypeOrDefault>>>>, ArchetypalAlias::ConstReferenceTrait, Iterator>;
      using DifferenceTypeOrDefault   = DetectedOrType<ArchetypalAlias::DefaultTrait<std::ptrdiff_t>, ArchetypalAlias::DifferenceTypeTrait, Iterator>;

      inline static constexpr bool HasValueType        = IsDetectedV<ArchetypalAlias::ValueTypeTrait, Iterator>;
      inline static constexpr bool HasContainerType    = IsDetectedV<ArchetypalAlias::ContainerTypeTrait, Iterator>;
      inline static constexpr bool HasIteratorCategory = IsDetectedV<ArchetypalAlias::IteratorCategoryTrait, Iterator>;
      inline static constexpr bool HasIteratorConcept  = IsDetectedV<ArchetypalAlias::IteratorConceptTrait, Iterator>;
      inline static constexpr bool HasPointer          = IsDetectedV<ArchetypalAlias::PointerTrait, Iterator>;
      inline static constexpr bool HasConstPointer     = IsDetectedV<ArchetypalAlias::ConstPointerTrait, Iterator>;
      inline static constexpr bool HasReference        = IsDetectedV<ArchetypalAlias::ReferenceTrait, Iterator>;
      inline static constexpr bool HasConstReference   = IsDetectedV<ArchetypalAlias::ConstReferenceTrait, Iterator>;
      inline static constexpr bool HasDifferenceType   = IsDetectedV<ArchetypalAlias::DifferenceTypeTrait, Iterator>;
  };
} // namespace Cxx::DetectionIdiom::Traits

namespace Cxx::Traits
{
  template <typename Iterator>
  struct IteratorTraits
  {
      using value_type        = DetectionIdiom::Traits::IteratorTraits<Iterator>::ValueTypeOrDefault;        // typename Iterator::value_type
      using container_type    = DetectionIdiom::Traits::IteratorTraits<Iterator>::ContainerTypeOrDefault;    // typename Iterator::container_type
      using iterator_category = DetectionIdiom::Traits::IteratorTraits<Iterator>::IteratorCategoryOrDefault; // typename Iterator::iterator_category
      using iterator_concept  = DetectionIdiom::Traits::IteratorTraits<Iterator>::IteratorConceptOrDefault;  // typename Iterator::iterator_concept
      using pointer           = DetectionIdiom::Traits::IteratorTraits<Iterator>::PointerOrDefault;          // typename Iterator::pointer
      using const_pointer     = DetectionIdiom::Traits::IteratorTraits<Iterator>::ConstPointerOrDefault;     // typename Iterator::const_pointer;
      using reference         = DetectionIdiom::Traits::IteratorTraits<Iterator>::ReferenceOrDefault;        // typename Iterator::reference
      using const_reference   = DetectionIdiom::Traits::IteratorTraits<Iterator>::ConstReferenceOrDefault;   // typename Iterator::const_reference;
      using difference_type   = DetectionIdiom::Traits::IteratorTraits<Iterator>::DifferenceTypeOrDefault;   // typename Iterator::difference_type                                         //
  };
} // namespace Cxx::Traits

namespace Cxx::Concepts
{
  // clang-format off

  template <typename T>
  concept Iterator = requires
  {
    typename T::value_type;
    typename T::iterator_category;
    typename T::pointer;
    typename T::reference;
    typename T::difference_type;
  };

  template <typename T>
  concept Cpp20Iterator = Iterator<T> and requires
  {
    typename T::iterator_concept;
    typename T::const_reference;
    typename T::const_pointer;
  };

  // clang-format on

  template <typename T>
  concept ContiguousIterator = Iterator<T> and std::contiguous_iterator<T>;

  template <typename T>
  concept RandomAccessIterator = Iterator<T> and std::random_access_iterator<T>;

  template <typename T>
  concept BidirectionalIterator = Iterator<T> and std::bidirectional_iterator<T>;

  template <typename T>
  concept ForwardIterator = Iterator<T> and std::forward_iterator<T>;

  template <typename T>
  concept InputIterator = Iterator<T> and std::input_iterator<T>;

  template <typename I, typename T>
  concept OutputIterator = Iterator<I> and std::output_iterator<I, T>;

  template <typename T>
  concept Cpp20ContiguousIterator = Cpp20Iterator<T> and std::contiguous_iterator<T>;

  template <typename T>
  concept Cpp20RandomAccessIterator = Cpp20Iterator<T> and std::random_access_iterator<T>;

  template <typename T>
  concept Cpp20BidirectionalIterator = Cpp20Iterator<T> and std::bidirectional_iterator<T>;

  template <typename T>
  concept Cpp20ForwardIterator = Cpp20Iterator<T> and std::forward_iterator<T>;

  template <typename T>
  concept Cpp20InputIterator = Cpp20Iterator<T> and std::input_iterator<T>;

  template <typename I, typename T>
  concept Cpp20OutputIterator = Cpp20Iterator<I> and std::output_iterator<I, T>;
} // namespace Cxx::Concepts

#endif /* A2427B5C_8C4F_4F4F_B256_CA7A977DA926 */
