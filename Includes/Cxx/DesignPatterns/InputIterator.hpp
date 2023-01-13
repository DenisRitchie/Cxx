#ifndef DB912288_044F_4887_9619_AF5E2B2D8953
#define DB912288_044F_4887_9619_AF5E2B2D8953

#include <iterator>

namespace Cxx
{
  template <typename InputIteratorDerived, typename ValueType>
  class InputIterator
  {
    public:
      using container_type    = void;
      using iterator_category = std::input_iterator_tag;
      using iterator_concept  = std::input_iterator_tag;
      using value_type        = ValueType;
      using pointer           = value_type*;
      using const_pointer     = const value_type*;
      using reference         = value_type&;
      using const_reference   = const value_type&;
      using difference_type   = std::ptrdiff_t;

    private:
      value_type& operator*()
      {
      }

      InputIterator& operator++()
      {
        return *this;
      }

      InputIterator operator++(int32_t)
      {
        return *this;
      }
  };

  template <typename Type>
  class InputIteratorBuilder
  {

    private:
  };
} // namespace Cxx

#endif /* DB912288_044F_4887_9619_AF5E2B2D8953 */
