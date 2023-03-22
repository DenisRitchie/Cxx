#ifndef CC4CC4EA_6005_4642_BC5F_855D9EC6C211
#define CC4CC4EA_6005_4642_BC5F_855D9EC6C211

#include "TypeTraits.hpp"

#include <utility>
#include <optional>
#include <tuple>
#include <array>
#include <ranges>
#include <string>
#include <string_view>
#include <span>
#include <sstream>
#include <variant>

namespace Cxx
{
  namespace Utilities
  {
    /**
     * @brief Desreferenciar un puntero de forma recursiva.
     *
     * @tparam T Tipo del puntero a desreferenciar.
     * @param pointer_or_value Puntero a desreferenciar.
     *
     * @return constexpr decltype(auto) Regresa el valor más profundo desreferenciado.
     */
    template <typename T>
    [[nodiscard]] inline constexpr decltype(auto) deep_raw_pointer_dereference(T&& pointer_or_value) noexcept;

    /**
     * @brief Desreferenciar un puntero inteligente de forma recursiva.
     *
     * @tparam T Tipo del puntero inteligente a desreferenciar.
     * @param pointer_or_value Puntero inteligente a desreferenciar.
     *
     * @return constexpr decltype(auto) Regresa el valor más profundo desreferenciado.
     */
    template <typename T>
    [[nodiscard]] inline constexpr decltype(auto) deep_smart_pointer_dereference(T&& pointer_or_value) noexcept;
  } // namespace Utilities

  /**
   * @brief Helper type for the visitor.
   *
   * @tparam Ts Each of the base classes for Overloaded.
   */
  template <class... Ts>
  struct Overloaded : Ts...
  {
      using Ts::operator()...; /**< Exposes operator() from every base. */
  };

  /**
   * @brief Explicit deduction guide (not needed as of C++20).
   *
   * @tparam Ts
   */
  template <class... Ts>
  Overloaded(Ts...) -> Overloaded<Ts...>;

  /**
   * @brief FakeCopyInit<T>(E): Helper function to evaluate expressions at Compile-Time.
   *
   *  (1) has type T [decay_t<decltype((E))> if T is deduced],
   *  (2) is well-formed if and only if E is implicitly convertible to T and T is destructible, and
   *  (3) is non-throwing if and only if both conversion from decltype((E)) to T and destruction of T are non-throwing.
   *
   * @tparam Type Data Type that is deduced in Compile-Time.
   * @return Type The return is only used to test whether the object is implicitly convertible and destructible.
   */
  template <typename Type>
  [[nodiscard]] Type FakeCopyInit(Type) noexcept;

  /**
   * @brief Obtiene el tipo de retorno de la función de transformación "Projection" que usa std::iter_value_t<Iterator> como argumento.
   *
   * @tparam Iterator   Tipo del iterador que será usado como argumento desreferenciado de "Projection".
   * @tparam Projection Función de transformación para std::iter_value_t<Iterator> a un nuevo tipo.
   */
  template <std::indirectly_readable Iterator, std::indirectly_regular_unary_invocable<Iterator> Projection>
  using projected_t = typename std::projected<Iterator, Projection>::value_type;

  /**
   * @brief Disable common object operations
   *
   *  Some overload sets in the library have the property that their constituent function templates are not visible
   *  to argument-dependent name lookup (ADL) and that they inhibit ADL when found via unqualified name lookup.
   *  This property allows these overload sets to be implemented as function objects. We derive such function
   *  objects from this type to remove some typical object-ish behaviors which helps users avoid depending on their
   *  non-specified object-ness.
   */
  class NotQuiteObject
  {
    public:
      struct ConstructTag
      {
          explicit ConstructTag() = default;
      };

      constexpr explicit NotQuiteObject(ConstructTag) noexcept {};

      NotQuiteObject()                                 = delete;
      NotQuiteObject(const NotQuiteObject&)            = delete;
      NotQuiteObject& operator=(const NotQuiteObject&) = delete;

      void operator&() const = delete;

    protected:
      ~NotQuiteObject() = default;
  };

  namespace Details::CustomizationPointObjects
  {
    /**
     * @brief Realiza una comparación de 3 vías para 2 objetos.
     */
    struct CompareThreeWayOrderFallback
    {
        /**
         * @brief Habilita la busqueda de claves de diferentes tipos para los contenedores asociativos.
         */
        using is_transparent = int32_t;

        /**
         * @brief Realiza una comparación de 3 vías para 2 objetos.
         *
         *  Se analiza el tipo para deducir si el retorno es: std::strong_ordering, std::weak_ordering ó std::partial_ordering.
         *
         * @tparam LeftType  Tipo del objeto 1 a comparar.
         * @tparam RightType Tipo del objeto 2 a comparar.
         * @param left  Objeto 1 a comparar. Como mínimo debe tener sobrecargado los operadores (<, ==).
         * @param right Objeto 2 a comparar. Como mínimo debe tener sobrecargado los operadores (<, ==).
         * @return Regresa un valor de tipo: std::strong_ordering, std::weak_ordering ó std::partial_ordering.
         */
        template <typename LeftType, typename RightType>
        inline constexpr auto operator()(LeftType&& left, RightType&& right) const noexcept -> Traits::common_comparison_category_t<LeftType, RightType>;
    };
  } // namespace Details::CustomizationPointObjects

  inline namespace CustomizationPointObjects
  {
    /**
     * @brief Realiza una comparación de 3 vías para 2 objetos.
     */
    inline constexpr Cxx::Details::CustomizationPointObjects::CompareThreeWayOrderFallback CompareThreeWayOrderFallback;
  } // namespace CustomizationPointObjects

  inline namespace FunctionObjects
  {
    /**
     * @brief Objeto función que representa la identidad de un objeto.
     *
     *  std::identity realiza un reenvio perfecto, regresando el valor sin alterar.
     */
    inline constexpr std::identity Identity;
  } // namespace FunctionObjects

  /**
   * @brief Clase usada como std::end(object) para las cadenas terminadas en Cero.
   *
   *  ZStringSentinel representa la marca de fin de texto para las cadenas terminadas en Cero.
   *  El final del texto es representado por el caracter '\0'.
   *
   *  Por ejemplo: std::begin("Texto") != ZStringSentinel{ }
   */
  struct zstring_sentinel
  {
      template <Concepts::Character CharType>
      inline constexpr bool operator==(const CharType* pointer) const noexcept;

      template <Concepts::Character CharType>
      inline constexpr bool operator!=(const CharType* pointer) const noexcept;
  };

  /**
   * @brief Clase usada como std::ranges::view para las cadenas terminadas en Cero.
   *
   * @tparam CharType Tipo del caracter de la cadena terminada en Cero.
   */
  template <Concepts::Character CharType>
  class ZString : std::ranges::view_interface<ZString<CharType>>
  {
    public:
      using value_type      = CharType;
      using pointer         = value_type*;
      using const_pointer   = const value_type*;
      using reference       = value_type&;
      using const_reference = const value_type&;
      using iterator        = value_type*;
      using const_iterator  = const value_type*;
      using difference_type = std::ptrdiff_t;
      using size_type       = std::size_t;

      constexpr ZString(const_pointer pointer) noexcept;
      constexpr ZString(const_pointer pointer, const size_type size) noexcept;

      template <size_t Size>
      constexpr ZString(const std::type_identity_t<value_type> (&array)[Size]) noexcept;

      template <typename TraitType, typename AllocType>
      constexpr ZString(const std::basic_string<CharType, TraitType, AllocType>& string) noexcept;

      template <typename TraitType>
      constexpr ZString(const std::basic_string_view<CharType, TraitType> string_view) noexcept;

      template <size_t Size>
      constexpr ZString(const std::array<CharType, Size>& array) noexcept;

      template <size_t Size>
      constexpr ZString(const std::span<CharType, Size>& span) noexcept;

      constexpr size_type size() const noexcept;

      constexpr const_iterator begin() const noexcept;
      constexpr const_iterator cbegin() const noexcept;

      constexpr const_iterator end() const noexcept;
      constexpr const_iterator cend() const noexcept;

      constexpr zstring_sentinel zend() const noexcept;
      constexpr zstring_sentinel zcend() const noexcept;

    private:
      const_pointer m_Pointer;
      size_type     m_Size;
  };

  inline namespace Literals
  {
    inline namespace StringLiterals
    {
      // clang-format off
      [[nodiscard]] inline constexpr ZString<char>     operator""_zs(const     char* string, const size_t length) noexcept;
      [[nodiscard]] inline constexpr ZString<wchar_t>  operator""_zs(const  wchar_t* string, const size_t length) noexcept;
      [[nodiscard]] inline constexpr ZString<char8_t>  operator""_zs(const  char8_t* string, const size_t length) noexcept;
      [[nodiscard]] inline constexpr ZString<char16_t> operator""_zs(const char16_t* string, const size_t length) noexcept;
      [[nodiscard]] inline constexpr ZString<char32_t> operator""_zs(const char32_t* string, const size_t length) noexcept;
      // clang-format on
    } // namespace StringLiterals
  }   // namespace Literals

  template <typename Type, template <typename...> class Template>
  inline constexpr bool is_specialization_v = false; // true if and only if Type is a specialization of Template

  template <template <typename...> class Template, typename... Types>
  inline constexpr bool is_specialization_v<Template<Types...>, Template> = true;

  template <typename>
  inline constexpr bool is_span_v = false;

  template <typename Type, size_t Extent>
  inline constexpr bool is_span_v<std::span<Type, Extent>> = true;

  template <typename>
  inline constexpr bool is_std_array_v = false;

  template <typename Type, size_t Size>
  inline constexpr bool is_std_array_v<std::array<Type, Size>> = true;

  namespace Traits
  {
    template <typename T, typename = void>
    struct StringTraits;

    template <std::ranges::contiguous_range T>
    requires(not requires { typename std::remove_cvref_t<T>::traits_type; }) // clang-format off
    struct StringTraits<T, std::void_t<std::ranges::range_value_t<std::remove_reference_t<T>>>> // clang-format on
    {
        using Type       = std::ranges::range_value_t<std::remove_reference_t<T>>;
        using TraitsType = std::char_traits<Type>;

        inline static constexpr bool IsInternalTrait = false;
    };

    template <std::ranges::contiguous_range T>
    struct StringTraits<T, std::void_t<std::ranges::range_value_t<std::remove_reference_t<T>>, typename std::remove_cvref_t<T>::traits_type>>
    {
        using Type       = std::ranges::range_value_t<std::remove_reference_t<T>>;
        using TraitsType = typename std::remove_cvref_t<T>::traits_type;

        inline static constexpr bool IsInternalTrait = true;
    };

    template <typename T>
    requires std::is_pointer_v<std::remove_cvref_t<T>>
    struct StringTraits<T, std::void_t<typename std::iterator_traits<std::remove_cvref_t<T>>::value_type>>
    {
        using Type       = typename std::iterator_traits<std::remove_cvref_t<T>>::value_type;
        using TraitsType = std::char_traits<Type>;

        inline static constexpr bool IsInternalTrait = false;
    };

    template <typename T>
    using CharacterTypeOf = typename Traits::StringTraits<T>::Type;

    template <typename T>
    using CharacterTraitsOf = typename Traits::StringTraits<T>::TraitsType;
  } // namespace Traits

  namespace Concepts
  {
    template <typename StringType>
    concept StringViewCompatible =                                   //
      requires { typename Traits::CharacterTypeOf<StringType>; } and //
      std::constructible_from<std::basic_string_view<Traits::CharacterTypeOf<StringType>, Traits::CharacterTraitsOf<StringType>>, StringType>;
  }

  template <std::ranges::input_range Range>
  struct PhonyInputIterator
  {
      using iterator_category = std::input_iterator_tag;
      using iterator_concept  = std::input_iterator_tag;
      using value_type        = std::ranges::range_value_t<Range>;
      using difference_type   = std::ptrdiff_t;
      using pointer           = std::add_pointer_t<std::ranges::range_reference_t<Range>>;
      using reference         = std::ranges::range_reference_t<Range>;

      reference operator*() const;
      pointer   operator->() const;

      PhonyInputIterator& operator++();
      PhonyInputIterator  operator++(int32_t);

      bool operator==(const PhonyInputIterator&) const;
  };
} // namespace Cxx

#include "Implementations/Utility.tcc"

#endif /* CC4CC4EA_6005_4642_BC5F_855D9EC6C211 */
