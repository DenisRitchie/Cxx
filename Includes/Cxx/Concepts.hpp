#ifndef B23D079A_2279_4C04_935A_45C2231630E4
#define B23D079A_2279_4C04_935A_45C2231630E4

#include <concepts>

namespace Cxx::Concepts
{
  namespace Details
  {
    template <class Type>
    concept BooleanTestableImpl = std::convertible_to<Type, bool>;
  }

  /**
   * @brief Concepto que verifica que un Tipo se puede convertir en Boolean.
   *
   *  Referencia: https://en.cppreference.com/w/cpp/concepts/boolean-testable
   *
   * @tparam Type Parámetro de tipo que se va ha evaluar.
   */
  template <class Type>
  concept BooleanTestable = // clang-format off
    Details::BooleanTestableImpl<Type>
    && requires(Type&& value)
    {
      { !std::forward<Type>(value) } -> Details::BooleanTestableImpl;
    };
  // clang-format on

  template <class Type1, class Type2>
  concept HalfEqualityComparable = // clang-format off
    requires(const std::remove_reference_t<Type1>& value1, const std::remove_reference_t<Type1>& value2)
    {
      { value1 == value2 } -> BooleanTestable;
      { value1 != value2 } -> BooleanTestable;
    };
  // clang-format on

  /**
   * @brief Concepto que verifica una comparación débil (==, !=) entre 2 tipos.
   *
   *  Referencia: https://en.cppreference.com/w/cpp/concepts/equality_comparable
   *
   * @tparam Type1 Operando Izquierdo.
   * @tparam Type2 Operando Derecho.
   */
  template <class Type1, class Type2>
  concept WeaklyEqualityComparableWith = HalfEqualityComparable<Type1, Type2> and HalfEqualityComparable<Type2, Type1>;

  template <class Type1, class Type2>
  concept HalfOrdered = // clang-format off
    requires(const std::remove_reference_t<Type1>& value1, const std::remove_reference_t<Type2>& value2)
    {
      { value1 <  value2 } -> BooleanTestable;
      { value1 >  value2 } -> BooleanTestable;
      { value1 <= value2 } -> BooleanTestable;
      { value1 >= value2 } -> BooleanTestable;
    };
  // clang-format on

  /**
   * @brief Concepto que verifica una comparación débil (<, >, <=, >=) entre 2 tipos.
   *
   *  Referencia: https://en.cppreference.com/w/cpp/concepts/totally_ordered
   *
   * @tparam Type1 Operando Izquierdo.
   * @tparam Type2 Operando Derecho.
   */
  template <class Type1, class Type2>
  concept PartiallyOrderedWith = HalfOrdered<Type1, Type2> and HalfOrdered<Type2, Type1>;

  /**
   * @brief Concepto que verifica si un tipo específico es una categoría válida de comparación.
   *
   *  Referencia: https://en.cppreference.com/w/cpp/utility/compare/three_way_comparable
   *
   * @tparam Type Tipo que se va ha verificar.
   * @tparam Category Tipo de la categoría ha evaluar.
   */
  template <class Type, class Category>
  concept ComparesAs = std::same_as<std::common_comparison_category_t<Type, Category>, Category>;

  template <class Type>
  using WithReference = Type&;

  template <class Type>
  concept CanReference = requires { typename WithReference<Type>; };

  template <class Type>
  concept Dereferenceable = requires(Type& value) // clang-format off
  {
    { *value } -> CanReference;
  }; // clang-format on

  template <class Type>
  concept HasMemberIteratorConcept = requires { typename Type::iterator_concept; };

  template <class Type>
  concept HasMemberIteratorCategory = requires { typename Type::iterator_category; };

  template <class Type>
  concept HasMemberValueType = requires { typename Type::value_type; };

  template <class Type>
  concept HasMemberElementType = requires { typename Type::element_type; };

  template <class Type>
  concept HasMemberDifferenceType = requires { typename Type::difference_type; };

  template <class Type>
  concept HasMemberPointer = requires { typename Type::pointer; };

  template <class Type>
  concept HasMemberReference = requires { typename Type::reference; };

  template <class Iterator>
  concept Cpp17Iterator = // clang-format off
    std::copyable<Iterator> and requires(Iterator it)
    {
      { *it   } -> CanReference;
      { ++it  } -> std::same_as<Iterator&>;
      { *it++ } -> CanReference;
    }; // clang-format on

  template <class Iterator>
  concept Cpp17InputIterator = // clang-format off
    Cpp17Iterator<Iterator>
    && std::equality_comparable<Iterator>
    && HasMemberDifferenceType<std::incrementable_traits<Iterator>>
    && HasMemberValueType<std::indirectly_readable_traits<Iterator>>
    && requires(Iterator it)
    {
        typename std::common_reference_t<std::iter_reference_t<Iterator>&&, typename std::indirectly_readable_traits<Iterator>::value_type&>;
        typename std::common_reference_t<decltype(*it++)&&, typename std::indirectly_readable_traits<Iterator>::value_type&>;
        requires std::signed_integral<typename std::incrementable_traits<Iterator>::difference_type>;
    }; // clang-format on

  template <typename Type>
  struct Choice
  {
      Type Strategy = Type{};
      bool NoThrow  = false;
  };
} // namespace Cxx::Concepts

#endif /* B23D079A_2279_4C04_935A_45C2231630E4 */
