#ifndef C1FFA0CD_B02C_44A3_8388_648C125FE042
#define C1FFA0CD_B02C_44A3_8388_648C125FE042

#include "Cxx/FunctionTraits.hpp"

namespace Cxx::DesignPatterns
{
  namespace Details
  {
    // clang-format off

    template <typename Function>
    concept CallableContainer = std::is_invocable_v<Function> and Traits::FunctionTraits<Function>::ArgumentCount == Traits::NumberOfArguments<1>;

    // clang-format on
  } // namespace Details

  template <Details::CallableContainer CallableContainer>
  class OutputIteratorAdapter
  {
    public:
      using ContainerType    = CallableContainer;
      using IteratorCategory = std::output_iterator_tag;
      using IteratorConcept  = std::output_iterator_tag;
      using ValueType        = Traits::FunctionTraits<ContainerType>::Argument<0>;
      using Pointer          = ValueType*;
      using ConstPointer     = const ValueType*;
      using Reference        = ValueType&;
      using ConstReference   = const ValueType&;
      using DifferenceType   = std::ptrdiff_t;

      using container_type    = ContainerType;
      using iterator_category = IteratorCategory;
      using iterator_concept  = IteratorConcept;
      using value_type        = ValueType;
      using pointer           = Pointer;
      using const_pointer     = ConstPointer;
      using reference         = Reference;
      using const_reference   = ConstReference;
      using difference_type   = DifferenceType;

      // clang-format off

      OutputIteratorAdapter& operator*() noexcept { return *this; }
      OutputIteratorAdapter& operator++() noexcept { return *this; }
      OutputIteratorAdapter& operator++(int32_t) noexcept { return *this; }

      // clang-format on

      OutputIteratorAdapter(ContainerType&& callback) noexcept
        : Callback{ std::forward<ContainerType>(callback) }
      {
      }

      OutputIteratorAdapter& operator=(const ValueType& value)
      {
        Callback(value);
        return *this;
      }

      OutputIteratorAdapter& operator=(ValueType&& value)
      {
        Callback(std::move(value));
        return *this;
      }

    private:
      ContainerType Callback;
  };
} // namespace Cxx::DesignPatterns

#endif /* C1FFA0CD_B02C_44A3_8388_648C125FE042 */
