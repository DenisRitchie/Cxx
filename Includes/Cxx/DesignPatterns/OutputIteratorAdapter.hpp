#ifndef C1FFA0CD_B02C_44A3_8388_648C125FE042
#define C1FFA0CD_B02C_44A3_8388_648C125FE042

#include <concepts>
#include <functional>

#include "Cxx/IteratorTraits.hpp"

namespace Cxx::DesignPatterns
{
  template <typename Function>
  class OutputIteratorAdapter
  {
    public:
      using ContainerType     = Function;
      using IteratorCategory  = std::output_iterator_tag;
      using IteratorConcept   = std::output_iterator_tag;
      using ValueType         = DetectionIdiom::Traits::IteratorTraits<ContainerType>::ValueTypeOrDefault;
      using Pointer           = DetectionIdiom::Traits::IteratorTraits<ContainerType>::PointerOrDefault;
      using ConstPointer      = DetectionIdiom::Traits::IteratorTraits<ContainerType>::ConstPointerOrDefault;
      using Reference         = DetectionIdiom::Traits::IteratorTraits<ContainerType>::ReferenceOrDefault;
      using ConstReference    = DetectionIdiom::Traits::IteratorTraits<ContainerType>::ConstReferenceOrDefault;
      using DifferenceType    = DetectionIdiom::Traits::IteratorTraits<ContainerType>::DifferenceTypeOrDefault;

      using container_type    = ContainerType;
      using iterator_category = IteratorCategory;
      using iterator_concept  = IteratorConcept;
      using value_type        = ValueType;
      using pointer           = Pointer;
      using const_pointer     = ConstPointer;
      using reference         = Reference;
      using const_reference   = ConstReference;
      using difference_type   = DifferenceType;

      OutputIteratorAdapter& operator*() noexcept
      {
        return *this;
      }

      OutputIteratorAdapter& operator++() noexcept
      {
        return *this;
      }

      OutputIteratorAdapter& operator++(int32_t) noexcept
      {
        return *this;
      }

      OutputIteratorAdapter(Function&& function) noexcept
        : m_Function{ std::forward<Function>(function) }
      {
      }

      template <typename Value>
      OutputIteratorAdapter& operator=(Value&& value)
      {
        m_Function(std::forward<Value>(value));
        return *this;
      }

    private:
      Function m_Function;
  };

  template <typename Function>
  OutputIteratorAdapter(Function&& function) -> OutputIteratorAdapter<Function>;
} // namespace Cxx::DesignPatterns

#endif /* C1FFA0CD_B02C_44A3_8388_648C125FE042 */
