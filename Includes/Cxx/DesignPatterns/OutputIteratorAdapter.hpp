// https://www.fluentcpp.com/2020/11/06/how-to-make-a-copyable-object-assignable-in-cpp/
// https://www.fluentcpp.com/2020/10/02/how-to-implement-operator-when-a-data-member-is-a-lambda/

#ifndef C1FFA0CD_B02C_44A3_8388_648C125FE042
#define C1FFA0CD_B02C_44A3_8388_648C125FE042

#include "Cxx/FunctionTraits.hpp"
#include "Cxx/SemiRegularBox.hpp"
#include <optional>

namespace Cxx::DesignPatterns
{
  template <typename CallableContainer>
  class OutputIteratorAdapter
  {
    public:
      using ContainerType    = CallableContainer;
      using IteratorCategory = std::output_iterator_tag;
      using IteratorConcept  = std::output_iterator_tag;
      using ValueType        = Cxx::Traits::RemoveAllSymbols<typename Traits::FunctionTraits<ContainerType>::template Argument<0>>;
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
        : Callback(std::move(callback))
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
      Helpers::semiregular_box_t<ContainerType> Callback;
  };

  template <typename CallableContainer>
  OutputIteratorAdapter<CallableContainer> MakeOutputIterator(CallableContainer&& function) noexcept
  {
    return OutputIteratorAdapter<CallableContainer>{ function };
  }

  template <typename CallableContainer, typename... ArgumentType>
  OutputIteratorAdapter<CallableContainer> MakeOutputIterator(ArgumentType&&... arguments) noexcept
  {
    return OutputIteratorAdapter<CallableContainer>{ CallableContainer(std::forward<ArgumentType>(arguments)...) };
  }
} // namespace Cxx::DesignPatterns

#endif /* C1FFA0CD_B02C_44A3_8388_648C125FE042 */
