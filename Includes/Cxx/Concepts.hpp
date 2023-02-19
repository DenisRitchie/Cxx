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
    requires(const remove_reference_t<Type1>& value1, const remove_reference_t<Type2>& value2)
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

  template <typename Type>
  struct Choice
  {
      Type Strategy = Type{};
      bool NoThrow  = false;
  };
} // namespace Cxx::Concepts

#endif /* B23D079A_2279_4C04_935A_45C2231630E4 */
