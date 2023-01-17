#ifndef B5A9594F_915C_489B_ACFA_0EC539CC64F8
#define B5A9594F_915C_489B_ACFA_0EC539CC64F8

#include "Utility.hpp"
#include "Coroutines/Generator.hpp"
#include "DesignPatterns/OutputIterator.hpp"
#include "DesignPatterns/InputIterator.hpp"
#include "Details/AlgorithmImpl.hpp"

#include <cstring>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <spanstream>

namespace Cxx::inline V1
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
  std::basic_string<CharType, TraitType, AllocType> Join(auto&& container, auto&& separator) // clang-format off
  requires requires(decltype(container) range)
  {
    { std::begin(range) } -> std::input_iterator;
    { std::end(range)   } -> std::input_iterator;
  } // clang-format on
  {
    using ContainerType = decltype(container);
    using SeparatorType = decltype(separator);

    std::basic_ostringstream<CharType, TraitType, AllocType> output;

    std::ranges::copy( //
      std::begin(std::forward<ContainerType>(container)),
      std::end(std::forward<ContainerType>(container)),
      Cxx::DesignPatterns::MakeOstreamJoiner(output, std::forward<SeparatorType>(separator))
    );

    return output.str();
  };

  // /**
  //  * @brief Función que convierte un string en un conjunto de tokens delimitados por un patrón dado.
  //  *
  //  * @tparam CharType  Tipo del caracter del string usando como contenido a tokenizar y el patrón dado.
  //  * @tparam TraitType Tipo del std::char_traits usado por el string del contenido a tokenizar y el patrón dado.
  //  * @param[in] content Contenido que se va a tokenizar.
  //  * @param[in] pattern Patrón que será usado como delimitador de cada token.
  //  * @see std::basic_string_view<CharType, TraitType>
  //  * @see std::char_traits<CharType>
  //  * @return std::ranges::split_view<std::basic_string_view<>, std::basic_string_view<>> Regresa un InputIterator que contiene cada token.
  //  */
  // template <typename CharType, typename TraitType>
  // inline decltype(auto) Split(const std::basic_string_view<CharType, TraitType> content, const std::basic_string_view<CharType, TraitType> pattern)
  // {
  //   return content | std::ranges::views::split(pattern);
  // }

  // /**
  //  * @brief Función que convierte un string en un conjunto de tokens delimitados por un patrón dado.
  //  *
  //  * @tparam CharType  Tipo del caracter del string usando como contenido a tokenizar y el patrón dado.
  //  * @param[in] content Contenido que se va a tokenizar.
  //  * @param[in] pattern Patrón que será usado como delimitador de cada token.
  //  * @return std::ranges::split_view<ZString<>, ZString<>> Regresa un InputIterator que contiene cada token.
  //  */
  // template <typename CharType>
  // inline decltype(auto) Split(const CharType* content, const CharType* pattern)
  // {
  //   return ZString{ content } | std::ranges::views::split(ZString{ pattern });
  // }

} // namespace Cxx::inline V1

namespace Cxx::inline V2
{
  // using V1::Join;
  // using V1::Split;

  namespace Details::Cpo
  {
    class CpoSplit
    {
      private:
        /**
         * @brief Método que permite escoger el contiguous_range adecuado.
         *
         * @tparam Type Tipo del contiguous_range a procesar.
         * @param value contiguous_range a procesar.
         * @return constexpr ZString<> | std::span<> Regresa un ZString<> o un std::span<> según el contiguous_range asociado.
         */
        template <typename Type>
        [[nodiscard]] static constexpr decltype(auto) Choose(Type&& value) noexcept
        {
          if constexpr ( std::is_pointer_v<std::remove_reference_t<Type>> || std::is_array_v<std::remove_reference_t<Type>> )
          {
            return ZString{ std::forward<Type>(value) };
          }
          else
          {
            // return std::span<typename std::remove_cvref_t<Type>::value_type>( //
            //   std::data(std::forward<Type>(value)),
            //   std::size(std::forward<Type>(value))
            // );

            return std::forward<Type>(value);
          }
        }

      public:
        /**
         * @brief Función usada para tokenizar un std::ranges::contiguous_range delimitado por un patrón.
         *
         * @tparam Range   Tipo del contenedor que se va a tokenizar. Debe cumplir el concepto: std::ranges::contiguous_range.
         * @tparam Pattern Tipo del contenedor que se usará como delimitador. Debe cumplir el concepto: std::ranges::contiguous_range.
         * @param range   std::ranges::contiguous_range que se va a tokenizar.
         * @param pattern std::ranges::contiguous_range que se usará como delimitador de cada token.
         * @return constexpr std::ranges::split_view<,> Regresa todos los tokens como un std::ranges::split_view<,>
         */
        template <std::ranges::contiguous_range Range, std::ranges::contiguous_range Pattern>
        [[nodiscard]] constexpr decltype(auto) operator()(Range&& range, Pattern&& pattern) const noexcept
        {
          return std::ranges::views::split( //
            Choose(std::forward<Range>(range)),
            Choose(std::forward<Pattern>(pattern))
          );

          // return Choose(std::forward<Range>(range)) | std::ranges::views::split(Choose(std::forward<Pattern>(pattern)));
        }
    };
  } // namespace Details::Cpo

  inline namespace CustomizationPointObjects
  {
    /**
     * @brief Objeto usando para tokenizar un contenedor en base a un patrón dado.
     *
     */
    inline constexpr Details::Cpo::CpoSplit Split;
  } // namespace CustomizationPointObjects

} // namespace Cxx::inline V2

#endif /* B5A9594F_915C_489B_ACFA_0EC539CC64F8 */
