// https://www.fluentcpp.com/2020/11/06/how-to-make-a-copyable-object-assignable-in-cpp/
// https://www.fluentcpp.com/2020/10/02/how-to-implement-operator-when-a-data-member-is-a-lambda/

#ifndef C1FFA0CD_B02C_44A3_8388_648C125FE042
#define C1FFA0CD_B02C_44A3_8388_648C125FE042

#include "Cxx/FunctionTraits.hpp"
#include "Cxx/SemiRegularBox.hpp"

#include <iterator>
#include <iosfwd>

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
        : m_Callback(std::move(callback))
      {
      }

      OutputIteratorAdapter& operator=(const ValueType& value)
      {
        m_Callback(value);
        return *this;
      }

      OutputIteratorAdapter& operator=(ValueType&& value)
      {
        m_Callback(std::move(value));
        return *this;
      }

    private:
      semiregular_box_t<ContainerType> m_Callback;
  };

  template <typename CallableContainer>
  inline OutputIteratorAdapter<CallableContainer> MakeOutputIterator(CallableContainer&& function) noexcept
  {
    return OutputIteratorAdapter<CallableContainer>{ std::forward<CallableContainer>(function) };
  }

  template <typename CallableContainer, typename... ArgumentType>
  inline OutputIteratorAdapter<CallableContainer> MakeOutputIterator(ArgumentType&&... arguments) noexcept
  {
    return OutputIteratorAdapter<CallableContainer>{ CallableContainer(std::forward<ArgumentType>(arguments)...) };
  }

  template <typename DelimiterType, typename CharType, typename TraitType = std::char_traits<CharType>>
  class OstreamJoiner
  {
    public:
      using ostream_type      = std::basic_ostream<CharType, TraitType>;
      using container_type    = ostream_type;
      using iterator_category = std::output_iterator_tag;
      using iterator_concept  = std::output_iterator_tag;
      using traits_type       = TraitType;
      using char_type         = CharType;
      using value_type        = char_type;
      using pointer           = value_type*;
      using const_pointer     = const value_type*;
      using reference         = value_type&;
      using const_reference   = const value_type&;
      using difference_type   = std::ptrdiff_t;

      // clang-format off

      OstreamJoiner& operator*() noexcept { return *this; }
      OstreamJoiner& operator++() noexcept { return *this; }
      OstreamJoiner& operator++(int32_t) noexcept { return *this; }

      // clang-format on

      OstreamJoiner(ostream_type& output, const DelimiterType& delimiter) noexcept(std::is_nothrow_copy_constructible_v<DelimiterType>)
        : m_Output(std::addressof(output))
        , m_Delimiter(delimiter)
      {
      }

      OstreamJoiner(ostream_type& output, DelimiterType&& delimiter) noexcept(std::is_nothrow_move_constructible_v<DelimiterType>)
        : m_Output(std::addressof(output))
        , m_Delimiter(std::move(delimiter))
      {
      }

      template <typename ValueType>
      OstreamJoiner& operator=(const ValueType& value)
      {
        if ( not m_First )
        {
          *m_Output << m_Delimiter;
        }

        m_First = false;
        *m_Output << value;
        return *this;
      }

    private:
      ostream_type* m_Output;
      DelimiterType m_Delimiter;
      bool          m_First{ true };
  };

  template <typename CharType, typename TraitType, typename DelimiterType>
  inline OstreamJoiner<std::decay_t<DelimiterType>, CharType, TraitType> MakeOstreamJoiner(std::basic_ostream<CharType, TraitType>& output, DelimiterType&& delimiter)
  {
    return { output, std::forward<DelimiterType>(delimiter) };
  }
} // namespace Cxx::DesignPatterns

#endif /* C1FFA0CD_B02C_44A3_8388_648C125FE042 */
