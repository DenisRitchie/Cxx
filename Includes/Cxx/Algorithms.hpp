/**
 * @file Algorithms.hpp
 * @author Denis West <DenisWest@outlook.com>
 * @brief Algoritmos de uso común y correcciones de la librería de Rangos de C++ en la manipulación de cadenas.
 * @version 1.0
 * @date 2023-02-25
 * @copyright Copyright (c) 2023
 */

#ifndef B5A9594F_915C_489B_ACFA_0EC539CC64F8
#define B5A9594F_915C_489B_ACFA_0EC539CC64F8

#include "Utility.hpp"
#include "Concepts.hpp"
#include "Coroutines/Generator.hpp"
#include "DesignPatterns/OutputIterator.hpp"
#include "DesignPatterns/InputIterator.hpp"

#include <algorithm>
#include <array>
#include <compare>
#include <functional>
#include <ranges>
#include <span>
#include <spanstream>
#include <string>
#include <string_view>
#include <numeric>
#include <tuple>
#include <iterator>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <locale>

#include <cstring>
#include <cctype>
#include <cuchar>
#include <ctime>

#if defined(__GNUC__)

namespace std
{
  struct from_range_t
  {
      explicit from_range_t() = default;
  };

  inline constexpr from_range_t from_range;
} // namespace std

namespace std::ranges
{
  template <class Derived>
  requires std::is_class_v<Derived> && std::same_as<Derived, remove_cv_t<Derived>>
  struct range_adaptor_closure : std::ranges::views::__adaptor::_RangeAdaptorClosure
  {
  };

# ifndef __cpp_lib_ranges_to_container
  namespace Details
  {
    // clang-format off
    template <class Range, class Container>
    concept SizedAndReservable = sized_range<Range> && sized_range<Container>
        && requires(Container& Cont, const range_size_t<Container> Count)
        {
            Cont.reserve(Count);
            { Cont.capacity() } -> same_as<range_size_t<Container>>;
            { Cont.max_size() } -> same_as<range_size_t<Container>>;
        };
    // clang-format on

    template <class Range, class Container>
    concept RefConverts = convertible_to<range_reference_t<Range>, range_value_t<Container>>;

    template <class Range, class Container, class... Types>
    concept ConvertsDirectConstructible = RefConverts<Range, Container> && constructible_from<Container, Range, Types...>;

    template <class Range, class Container, class... Types>
    concept ConvertsTagConstructible = RefConverts<Range, Container> && constructible_from<Container, const from_range_t&, Range, Types...>;

    template <class Range, class Container, class... Types>
    concept ConvertsAndCommonConstructible =                  //
      RefConverts<Range, Container> && common_range<Range>    //
      && Cxx::Concepts::Cpp17InputIterator<iterator_t<Range>> //
      && constructible_from<Container, iterator_t<Range>, iterator_t<Range>, Types...>;

    template <class Container, class Reference>
    concept CanPushBack = requires(Container& Cont) { Cont.push_back(std::declval<Reference>()); };

    template <class Container, class Reference>
    concept CanInsertEnd = requires(Container& Cont) { Cont.insert(Cont.end(), std::declval<Reference>()); };

    // clang-format off
    template <class Range, class Container, class... Types>
    concept ConvertsConstructibleInsertable = RefConverts<Range, Container>
        && constructible_from<Container, Types...>
        && (CanPushBack<Container, range_reference_t<Range>> || CanInsertEnd<Container, range_reference_t<Range>>); // clang-format on

    template <class Reference, class Container>
    [[nodiscard]] constexpr auto ContainerInserter(Container& Cont)
    {
      if constexpr ( Details::CanPushBack<Container, Reference> )
      {
        return back_insert_iterator{ Cont };
      }
      else
      {
        return insert_iterator{ Cont, Cont.end() };
      }
    }
  } // namespace Details

  template <class Container, input_range Range, class... Types>
  requires(not view<Container>)
  [[nodiscard]] constexpr Container to(Range&& range, Types&&... args)
  {
    if constexpr ( Details::ConvertsDirectConstructible<Range, Container, Types...> )
    {
      return Container(std::forward<Range>(range), std::forward<Types>(args)...);
    }
    else if constexpr ( Details::ConvertsTagConstructible<Range, Container, Types...> )
    {
      return Container(from_range, std::forward<Range>(range), std::forward<Types>(args)...);
    }
    else if constexpr ( Details::ConvertsAndCommonConstructible<Range, Container, Types...> )
    {
      return Container(std::ranges::begin(range), std::ranges::end(range), std::forward<Types...>(args)...);
    }
    else if constexpr ( Details::ConvertsConstructibleInsertable<Range, Container, Types...> )
    {
      Container Cont(std::forward<Types>(args)...);

      if constexpr ( Details::SizedAndReservable<Range, Container> )
      {
        Cont.reserve(std::ranges::size(range));
      }

      std::ranges::copy(range, Details::ContainerInserter<range_reference_t<Range>>(Cont));
      return Cont;
    }
    else if constexpr ( input_range<range_reference_t<Range>> )
    {
      const auto Xform = [](auto&& element)
      {
        return std::ranges::to<range_value_t<Container>>(std::forward<decltype(element)>(element));
      };

      return std::ranges::to<Container>(views::transform(range, Xform), std::forward<Types>(args)...);
    }
    else
    {
      static_assert(Cxx::Traits::AlwaysFalse<Container>, "the program is ill-formed per N4910 [range.utility.conv.to]/1.3");
    }
  }

  namespace Details
  {
    template <class Container>
    struct ToClassFunction
    {
        static_assert(not view<Container>, "not view<Container>");

        template <input_range Range, class... Types>
        [[nodiscard]] constexpr auto operator()(Range&& range, Types&&... args) const
        requires requires { std::ranges::to<Container>(std::forward<Range>(range), std::forward<Types>(args)...); }
        {
          return std::ranges::to<Container>(std::forward<Range>(range), std::forward<Types>(args)...);
        }
    };
  } // namespace Details

  template <class Container, class... Types>
  requires(not view<Container>)
  [[nodiscard]] constexpr auto to(Types&&... args)
  {
    return std::ranges::views::__adaptor::_Partial<Details::ToClassFunction<Container>, decay_t<Types>...>{ std::forward<Types>(args)... };
  }

  namespace Details
  {
    template <template <class...> class Container, class Range, class... Args>
    auto ToHelper()
    {
      if constexpr ( requires { Container(std::declval<Range>(), std::declval<Args>()...); } )
      {
        return static_cast<decltype(Container(std::declval<Range>(), std::declval<Args>()...))*>(nullptr);
      }
      else if constexpr ( requires { Container(from_range, std::declval<Range>(), std::declval<Args>()...); } )
      {
        return static_cast<decltype(Container(from_range, std::declval<Range>(), std::declval<Args>()...))*>(nullptr);
      }
      else if constexpr ( requires { Container(std::declval<Cxx::PhonyInputIterator<Range>>(), std::declval<Cxx::PhonyInputIterator<Range>>(), std::declval<Args>()...); } )
      {
        return static_cast<decltype(Container(std::declval<Cxx::PhonyInputIterator<Range>>(), std::declval<Cxx::PhonyInputIterator<Range>>(), std::declval<Args>()...))*>(nullptr);
      }
    }
  } // namespace Details

  template <template <class...> class Container, input_range Range, class... Types, class Deduced = remove_pointer_t<decltype(Details::ToHelper<Container, Range, Types...>())>>
  [[nodiscard]] constexpr Deduced to(Range&& range, Types&&... args)
  {
    return std::ranges::to<Deduced>(std::forward<Range>(range), std::forward<Types>(args)...);
  }

  namespace Details
  {
    template <template <class...> class Container>
    struct ToTemplateFunction
    {
        template <input_range Range, class... Types, class Deduced = remove_pointer_t<decltype(ToHelper<Container, Range, Types...>())>>
        [[nodiscard]] constexpr auto operator()(Range&& range, Types&&... args) const
        {
          return std::ranges::to<Deduced>(std::forward<Range>(range), std::forward<Types>(args)...);
        }
    };
  } // namespace Details

  template <template <class...> class Container, class... Types>
  [[nodiscard]] constexpr auto to(Types&&... args)
  {
    return std::ranges::views::__adaptor::_Partial<Details::ToTemplateFunction<Container>, decay_t<Types>...>{ std::forward<Types>(args)... };
  }
# endif // __cpp_lib_ranges_to_container
} // namespace std::ranges
#endif

namespace Cxx
{
  namespace Algorithms
  {
    /**
     * @brief Specifies whether applicable Overload:Cxx::Algorithms::Split method overloads include or omit empty substrings from the return value.
     */
    enum class StringSplitOptions
    {
      /**
       * @brief The return value includes array elements that contain an empty string.
       */
      None = 0x0,

      /**
       * @brief The return value does not include array elements that contain an empty string.
       */
      RemoveEmptyEntries = 0x1
    };

    namespace Views
    {
      /**
       * @brief View que permite ignorar los valores vacíos de un Rango.
       */
      struct IgnoreEmptyValuesAdaptorClosure : std::ranges::range_adaptor_closure<IgnoreEmptyValuesAdaptorClosure>
      {
          template <std::ranges::viewable_range Range>
          requires std::ranges::viewable_range<std::ranges::range_value_t<Range>> and // clang-format off
                   requires(std::ranges::range_value_t<Range>&& value)
                   {
                     { value.empty() } -> Concepts::BooleanTestable;
                   } // clang-format on
          [[nodiscard]] constexpr auto operator()(Range&& range) const noexcept
          {
            return std::forward<Range>(range) | std::views::filter([](auto&& value) { return not value.empty(); });
          }
      };

      inline constexpr IgnoreEmptyValuesAdaptorClosure IgnoreEmptyValues{};

      struct ValuesToStringViewAdaptorClosure : std::ranges::range_adaptor_closure<ValuesToStringViewAdaptorClosure>
      {
          template <std::ranges::viewable_range Range>
          requires std::ranges::viewable_range<std::ranges::range_value_t<Range>> and Concepts::StringViewCompatible<std::ranges::range_value_t<Range>>
          [[nodiscard]] constexpr auto operator()(Range&& range) const noexcept
          {
            using subrange_t    = std::ranges::range_value_t<Range>;
            using value_type_t  = std::ranges::range_value_t<subrange_t>;
            using string_view_t = std::basic_string_view<value_type_t>;

            return std::forward<Range>(range) | std::views::transform([](auto&& value) { return string_view_t(value.begin(), value.end()); });
          }
      };

      inline constexpr ValuesToStringViewAdaptorClosure ValuesToStringView{};
    } // namespace Views

  } // namespace Algorithms

  namespace Views = Algorithms::Views;
} // namespace Cxx

namespace Cxx::Algorithms::inline V1
{
  /**
   * @brief Convierte un contenedor en una cadena donde cada elemento está separado por un delimitador.
   *
   * @tparam ContainerType El tipo del input_container que contiene todos los elementos a unir en una cadena.
   * @tparam SeparatorType El tipo del separador usado para cada elemento del container.
   * @tparam CharType  El tipo del caracter del objeto string que se usará como retorno.
   * @tparam TraitType El tipo del std::char_trait<CharType> del objeto string que se usará como retorno.
   * @tparam AllocType El tipo del std::allocator<CharType> del objeto string que se usará como retorno.
   *
   * @param[in] container  Referencia del del input_container que contiene los elementos a unir en una cadena.
   * @param[in] separator  Referencia del separador de cada elemento del container.
   *
   * @return Regresa en un string todos los valores del contenedor separados por un delimitador.
   */
  template <typename CharType = char, typename TraitType = std::char_traits<CharType>, typename AllocType = std::allocator<CharType>>
  std::basic_string<CharType, TraitType, AllocType> Join(auto&& container, auto&& separator);

  /**
   * @brief Separa una Cadena en una lista de Tokens según el Patrón indicado.
   *
   * @tparam Options Opción para indicar si se debe regresar un rango con valores vacíos o sin ellos.
   *
   * @param[in] Text    Cadena que será separada en Tokens.
   * @param[in] Pattern Cadena usada como Patrón para separar la Cadena Text en Tokens.
   *
   * @return Regresa una lista de std::basic_string_view<CharType, TraitType> con todos los Tokens.
   */
  template <StringSplitOptions Options = StringSplitOptions::RemoveEmptyEntries>
  [[nodiscard]] inline constexpr auto Split(Concepts::StringViewCompatible auto&& Text, Concepts::StringViewCompatible auto&& Pattern) noexcept
  requires std::same_as<Traits::CharacterTypeOf<decltype(Text)>, Traits::CharacterTypeOf<decltype(Pattern)>>
  {
    using enum StringSplitOptions;
    using StringView = std::basic_string_view<Traits::CharacterTypeOf<decltype(Text)>, Traits::CharacterTraitsOf<decltype(Text)>>;

    auto&& tokens = StringView{ Text } | std::views::split(StringView{ Pattern }) | Views::ValuesToStringView;

    if constexpr ( Options == RemoveEmptyEntries )
    {
      return tokens | Views::IgnoreEmptyValues;
    }
    else
    {
      return tokens;
    }
  }

  namespace Details::FunctionObjects
  {
    /**
     * @brief Objeto función que compara 2 Rangos de categoría mínima: std::input_iterator.
     */
    class RangeCompare : public NotQuiteObject
    {
      private:
        /**
         * @brief Este valor es usado como UINT64_MAX para aquellas sobrecargas que no provean un número determinado de elementos a comparar.
         */
        inline static constexpr size_t DefaultSize = static_cast<size_t>(-1);

      public:
        using NotQuiteObject::NotQuiteObject;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         *  Cada rango debe proveer un "Begin" y "End" según el estandar de C++.
         *  El tipo del "End" no tiene que coincidir con el tipo del "Begin", "End" puede ser un Sentinela.
         *
         *  Para cada rango se debe proveer un método de proyección para transformar los datos antes de ser comparados.
         *  Y por último se debe proporcionar un método que compare cada valor del rango usando cualquier mecanismo de comparación de 3 vías de C++.
         *
         *  Este método de comparación debe regresar unos de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         *  Este método regresa un valor que representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         *
         * @tparam LeftFirst std::input_iterator del primer elemento del Rango 1.
         * @tparam LeftLast  std::sentinel_for<LeftFirst> del elemento siguiente al último del Rango 1.
         * @tparam RightFirst std::input_iterator del primer elemento del Rango 2.
         * @tparam RightLast  std::sentinel_for<RightFirst> del elemento siguiente al último del Rango 2.
         * @tparam LeftProjection  std::invocable<std::iter_value_t<LeftFirst>> Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @tparam RightProjection std::invocable<std::iter_value_t<RightFirst>> Método de transformación para cada valor del Rango 2 antes de ser comparado.
         * @tparam CompareThreeWay std::invocable<projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>> Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @param[in] left_first Iterador del primer elemento del Rango 1.
         * @param[in] left_last  Iterador/Sentinela del elemento siguiente al último del Rango 1.
         * @param[in] right_first Iterador del primer elemento del Rango 2.
         * @param[in] right_last  Iterador/Sentinela del elemento siguiente al último del Rango 2.
         * @param[in] number_of_items_to_compare Cantidad de elementos a comparar. Usar "-1" si se van a comparar todos los elementos.
         * @param[in] left_projection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @param[in] right_projection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         * @param[in] compare_three_way_order_fallback Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering) según el argumento compare_three_way_order_fallback. Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template < // clang-format off
          std::input_iterator          LeftFirst,
          std::sentinel_for<LeftFirst> LeftLast,
          std::input_iterator           RightFirst,
          std::sentinel_for<RightFirst> RightLast,
          std::invocable<std::iter_value_t<LeftFirst>>  LeftProjection,
          std::invocable<std::iter_value_t<RightFirst>> RightProjection,
          std::invocable<projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>> CompareThreeWay>
        constexpr auto operator()(
          LeftFirst&& left_first, LeftLast&& left_last, RightFirst&& right_first, RightLast&& right_last,
          size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection,
          CompareThreeWay&& compare_three_way_order_fallback
        ) const noexcept(noexcept(std::invoke(
            std::forward<CompareThreeWay>(compare_three_way_order_fallback),
              std::invoke(std::forward<LeftProjection>(left_projection), *left_first),
              std::invoke(std::forward<RightProjection>(right_projection), *right_first)
          ))) -> std::invoke_result_t<CompareThreeWay, projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>>;
        // clang-format on

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] number_of_items_to_compare Cantidad de elementos a comparar. Usar "-1" si se van a comparar todos los elementos.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam CompareThreeWay Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] compare_three_way_order_fallback Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>, std::ranges::range_value_t<RightRange>> CompareThreeWay>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam LeftProjection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @tparam RightProjection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] left_projection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @param[in] right_projection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, LeftProjection&& left_projection, RightProjection&& right_projection) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam LeftProjection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @tparam RightProjection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         * @tparam CompareThreeWay Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] left_projection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @param[in] right_projection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         * @param[in] compare_three_way_order_fallback Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, LeftProjection>, projected_t<std::ranges::iterator_t<RightRange>, RightProjection>> CompareThreeWay>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, LeftProjection&& left_projection, RightProjection&& right_projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam CompareThreeWay Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] number_of_items_to_compare Cantidad de elementos a comparar. Usar "-1" si se van a comparar todos los elementos.
         * @param[in] compare_three_way_order_fallback Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>, std::ranges::range_value_t<RightRange>> CompareThreeWay>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam LeftProjection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @tparam RightProjection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] number_of_items_to_compare Cantidad de elementos a comparar. Usar "-1" si se van a comparar todos los elementos.
         * @param[in] left_projection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @param[in] right_projection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam LeftProjection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @tparam RightProjection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         * @tparam CompareThreeWay Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] number_of_items_to_compare Cantidad de elementos a comparar. Usar "-1" si se van a comparar todos los elementos.
         * @param[in] left_projection  Método de transformación para cada valor del Rango 1 antes de ser comparado.
         * @param[in] right_projection Método de transformación para cada valor del Rango 2 antes de ser comparado.
         * @param[in] compare_three_way_order_fallback Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, LeftProjection>, projected_t<std::ranges::iterator_t<RightRange>, RightProjection>> CompareThreeWay>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam Projection Método de transformación para cada valor del Rango antes de ser comparado.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] projection  Método de transformación para cada valor del Rango antes de ser comparado.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection>
        requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, Projection&& projection) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam Projection Método de transformación para cada valor del Rango antes de ser comparado.
         * @tparam CompareThreeWay Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] projection  Método de transformación para cada valor del Rango antes de ser comparado.
         * @param[in] compare_three_way_order_fallback Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, Projection>, projected_t<std::ranges::iterator_t<RightRange>, Projection>> CompareThreeWay>
        requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, Projection&& projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam Projection Método de transformación para cada valor del Rango antes de ser comparado.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] number_of_items_to_compare Cantidad de elementos a comparar. Usar "-1" si se van a comparar todos los elementos.
         * @param[in] projection Método de transformación para cada valor del Rango antes de ser comparado.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection>
        requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, Projection&& projection) const noexcept;

        /**
         * @brief Compara 2 rangos de categoría mínima: std::input_iterator.
         *
         * @tparam LeftRange  Rango de tipo std::ranges::input_range.
         * @tparam RightRange Rango de tipo std::ranges::input_range.
         * @tparam Projection Método de transformación para cada valor del Rango antes de ser comparado.
         * @tparam CompareThreeWay Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @param[in] left_range  Rango 1 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] right_range Rango 2 de categoría: std::ranges::input_range que se va ha comparar.
         * @param[in] number_of_items_to_compare Cantidad de elementos a comparar. Usar "-1" si se van a comparar todos los elementos.
         * @param[in] projection Método de transformación para cada valor del Rango antes de ser comparado.
         * @param[in] compare_three_way_order_fallback Método de comparación de 3 vías. Debe retornar uno de los siguientes tipos: std::strong_ordering, std::weak_ordering, std::partial_ordering.
         *
         * @see Cxx::CompareThreeWayOrderFallback
         * @return Regresa una valor de tipo (std::strong_ordering, std::weak_ordering, std::partial_ordering). Este valor representa si el Rango 1 es (>, >=, <, <=, ==, !=) que el Rango 2.
         */
        template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, Projection>, projected_t<std::ranges::iterator_t<RightRange>, Projection>> CompareThreeWay>
        requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
        constexpr auto operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, Projection&& projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept;
    };
  } // namespace Details::FunctionObjects

  /**
   * @brief Objeto función que compara 2 Rangos de categoría mínima: std::input_iterator.
   */
  inline constexpr Details::FunctionObjects::RangeCompare RangeCompare{ NotQuiteObject::ConstructTag{} };
} // namespace Cxx::Algorithms::inline V1

#include "Implementations/Algorithms.tcc"

#endif /* B5A9594F_915C_489B_ACFA_0EC539CC64F8 */
