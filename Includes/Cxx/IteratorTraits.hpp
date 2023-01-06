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

  template <typename Iterator>
  struct IteratorTraits
  {
      inline static constexpr bool HasValueType      = IsDetectedV<ArchetypalAlias::ValueTypeTrait, Iterator>;
      inline static constexpr bool HasPointer        = IsDetectedV<ArchetypalAlias::PointerTrait, Iterator>;
      inline static constexpr bool HasConstPointer   = IsDetectedV<ArchetypalAlias::ConstPointerTrait, Iterator>;
      inline static constexpr bool HasReference      = IsDetectedV<ArchetypalAlias::ReferenceTrait, Iterator>;
      inline static constexpr bool HasConstReference = IsDetectedV<ArchetypalAlias::ConstReferenceTrait, Iterator>;
      inline static constexpr bool HasDifferenceType = IsDetectedV<ArchetypalAlias::DifferenceTypeTrait, Iterator>;

      using ValueTypeOrDefault                       = DetectedOrType<ArchetypalAlias::DefaultTrait<void>, ArchetypalAlias::ValueTypeTrait, Iterator>;
      using PointerOrDefault                         = DetectedOrType<ArchetypalAlias::DefaultTrait<ValueTypeOrDefault*>, ArchetypalAlias::PointerTrait, Iterator>;
      using ConstPointerOrDefault                    = DetectedOrType<ArchetypalAlias::DefaultTrait<const ValueTypeOrDefault*>, ArchetypalAlias::ConstPointerTrait, Iterator>;
      using ReferenceOrDefault                       = DetectedOrType<ArchetypalAlias::DefaultTrait<std::conditional_t<std::is_void_v<ValueTypeOrDefault>, PointerOrDefault, ValueTypeOrDefault>&>, ArchetypalAlias::ReferenceTrait, Iterator>;
      using ConstReferenceOrDefault                  = DetectedOrType<ArchetypalAlias::DefaultTrait<const std::conditional_t<std::is_void_v<ValueTypeOrDefault>, PointerOrDefault, ValueTypeOrDefault>&>, ArchetypalAlias::ConstReferenceTrait, Iterator>;
      using DifferenceTypeOrDefault                  = DetectedOrType<ArchetypalAlias::DefaultTrait<std::ptrdiff_t>, ArchetypalAlias::DifferenceTypeTrait, Iterator>;
  };
} // namespace Cxx::DetectionIdiom::Traits

#endif /* A2427B5C_8C4F_4F4F_B256_CA7A977DA926 */
