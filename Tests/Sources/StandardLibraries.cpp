#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <array>
#include <algorithm>
#include <charconv>
#include <chrono>
#include <concepts>
#include <future>
#include <iostream>
#include <iomanip>
#include <map>
#include <numeric>
#include <ranges>
#include <set>
#include <span>
#include <spanstream>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  template <typename T>
  void TypeIdentityFunctionCase1(T, std::type_identity_t<T>)
  {
  }

  template <typename CharType, typename TraitType>
  void TypeIdentityFunctionCase2(const std::basic_string_view<CharType, TraitType>, const std::type_identity_t<std::basic_string_view<CharType, TraitType>>)
  {
  }

  template <typename CharType, typename TraitType>
  void TypeIdentityFunctionCase3(const std::type_identity_t<std::basic_string_view<CharType, TraitType>>, const std::basic_string_view<CharType, TraitType>)
  {
  }

  template <typename CharType, typename TraitType>
  void TypeIdentityFunctionCase4(const std::type_identity_t<std::basic_string_view<CharType, TraitType>>, const std::type_identity_t<std::basic_string_view<CharType, TraitType>>)
  {
  }

  template <typename T, typename = void>
  struct StringTraits;

  template <std::ranges::contiguous_range T>
  requires not requires { typename std::remove_cvref_t<T>::traits_type; } // clang-format off
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
  using StringTraitsT = typename StringTraits<T>::Type;

  template <typename T>
  using StringTraitsTT = typename StringTraits<T>::TraitsType;

  template <typename CharType, typename TraitType>
  void TypeIdentityFunctionCase5Impl(const std::basic_string_view<CharType, TraitType> string1, const std::basic_string_view<CharType, TraitType> string2)
  {
    EXPECT_EQ(string1.size(), 4);
    EXPECT_EQ(string2.size(), 4);
    EXPECT_EQ(string1, string2);
  }

  template <typename StringType>
  concept StringViewCompatible =                         //
    requires { typename StringTraitsT<StringType>; } and //
    std::constructible_from<std::basic_string_view<StringTraitsT<StringType>, StringTraitsTT<StringType>>, StringType>;

  template <StringViewCompatible T, StringViewCompatible U>
  inline constexpr void TypeIdentityFunctionCase5(T&& string1, U&& string2)
  requires std::same_as<StringTraitsT<T>, StringTraitsT<U>>
  {
    using StringView = std::basic_string_view<StringTraitsT<T>, StringTraitsTT<T>>;
    TypeIdentityFunctionCase5Impl(StringView{ std::forward<T>(string1) }, StringView{ std::forward<U>(string2) });
  }

  template <typename T>
  void StringTraitsTests()
  {
    using CharType   = typename StringTraits<T>::Type;
    using TraitsType = typename StringTraits<T>::TraitsType;

    EXPECT_TRUE((std::same_as<CharType, char>));
    EXPECT_TRUE((std::same_as<typename TraitsType::char_type, char>));

    if constexpr ( not StringTraits<T>::IsInternalTrait )
    {
      EXPECT_TRUE((std::same_as<TraitsType, std::char_traits<CharType>>));
    }
    else
    {
      EXPECT_TRUE(( //
        requires {
          typename TraitsType::char_type;
          typename TraitsType::int_type;
          typename TraitsType::pos_type;
          typename TraitsType::off_type;
          typename TraitsType::state_type;
          typename TraitsType::comparison_category;
        }
      ));

      if constexpr ( requires { typename TraitsType::comparison_category; } )
      {
        EXPECT_TRUE(( //
          std::disjunction_v<
            std::is_same<typename TraitsType::comparison_category, std::weak_ordering>,
            std::is_same<typename TraitsType::comparison_category, std::partial_ordering>,
            std::is_same<typename TraitsType::comparison_category, std::strong_ordering>>));
      }

      if constexpr ( requires { typename TraitsType::int_type; } )
      {
        EXPECT_TRUE((std::integral<typename TraitsType::int_type>));
      }

      if constexpr ( requires { typename TraitsType::pos_type; } )
      {
        EXPECT_TRUE((std::same_as<typename TraitsType::pos_type, std::streampos>));
      }

      if constexpr ( requires { typename TraitsType::off_type; } )
      {
        EXPECT_TRUE((std::same_as<typename TraitsType::off_type, std::streamoff>));
      }
    }
  }

  TEST(StandardLibraryTests, TypeIdentity_Case5_StringTraits)
  {
    StringTraitsTests<char*>();
    StringTraitsTests<char*&>();
    StringTraitsTests<char*&&>();
    StringTraitsTests<const char*>();
    StringTraitsTests<const char*&>();
    StringTraitsTests<const char*&>();
    StringTraitsTests<char[5]>();
    StringTraitsTests<char(&)[5]>();
    StringTraitsTests<char(&&)[5]>();
    StringTraitsTests<const char[5]>();
    StringTraitsTests<const char(&)[5]>();
    StringTraitsTests<const char(&&)[5]>();
    StringTraitsTests<std::string>();
    StringTraitsTests<std::string&>();
    StringTraitsTests<std::string&&>();
    StringTraitsTests<const std::string>();
    StringTraitsTests<const std::string&>();
    StringTraitsTests<const std::string&&>();
    StringTraitsTests<std::string_view>();
    StringTraitsTests<std::string_view&>();
    StringTraitsTests<std::string_view&&>();
    StringTraitsTests<const std::string_view>();
    StringTraitsTests<const std::string_view&>();
    StringTraitsTests<const std::string_view&&>();
    StringTraitsTests<std::array<char, 5>>();
    StringTraitsTests<std::array<char, 5>&>();
    StringTraitsTests<std::array<char, 5>&&>();
    StringTraitsTests<const std::array<char, 5>>();
    StringTraitsTests<const std::array<char, 5>&>();
    StringTraitsTests<const std::array<char, 5>&&>();
    StringTraitsTests<std::vector<char>>();
    StringTraitsTests<std::vector<char>&>();
    StringTraitsTests<std::vector<char>&&>();
    StringTraitsTests<const std::vector<char>>();
    StringTraitsTests<const std::vector<char>&>();
    StringTraitsTests<const std::vector<char>&&>();
    StringTraitsTests<char* const>();
    StringTraitsTests<char* const&>();
    StringTraitsTests<char* const&&>();
    StringTraitsTests<const char* const>();
    StringTraitsTests<const char* const&>();
    StringTraitsTests<const char* const&>();
    StringTraitsTests<std::span<char>>();
    StringTraitsTests<std::span<char>&>();
    StringTraitsTests<std::span<char>&&>();
    StringTraitsTests<std::span<const char>>();
    StringTraitsTests<std::span<const char>&>();
    StringTraitsTests<std::span<const char>&&>();
    StringTraitsTests<const std::span<char>>();
    StringTraitsTests<const std::span<char>&>();
    StringTraitsTests<const std::span<char>&&>();
    StringTraitsTests<const std::span<const char>>();
    StringTraitsTests<const std::span<const char>&>();
    StringTraitsTests<const std::span<const char>&&>();
  }

  /**
   * @brief Construct a new TEST object
   *
   *  Reference: https://en.cppreference.com/w/cpp/language/template_argument_deduction
   */
  TEST(StandardLibraryTests, TypeIdentity)
  {
    constexpr const char*           Text = "Text";
    constexpr std::array<char, 4>   TextArray{ 'T', 'e', 'x', 't' };
    constexpr std::span<const char> TextSpan{ Text, Text + 4 };
    const std::vector<char>         TextVector{ 'T', 'e', 'x', 't' };

    // std::type_identity_t<T>
    // Permite deducir ciertas expresiones en código genérico que normalmente el compilador no puede detectar.
    //
    // Por ejemplo:
    //  F<T>(T, T) donde una posible llamada es: F(10,  36.5)
    //  Acá encontramos un error de deducción para "T" ya que el argumento 1 es int32_t y el argumento 2 es double.
    //  C++ no sabe cual de los 2 tipos deducir para "T".
    //
    // Pero si:
    //  F<T>(T, std::type_identity_t<T>) donde una posible llamada es: F(10, 36.5)
    //  C++ estable el argumento del tipo genérico "std::type_identity<T>" exactamente al mismo tipo del argumento 1,
    //  evitando que "T" traté de interpretarse como 2 tipos distintos, por tanto, podemos decir que obtuvimos un tipo de identidad.
    //
    TypeIdentityFunctionCase1(12, static_cast<int32_t>(36.3)); // El cast es necesario para evitar una advertencia que será marcada como error.

    // En el siguiente escenario tenemos 2 std::string_view
    // Al igual que pasaba con el caso anterior, C++ no es capaz de deducir la conversión explícita de:
    // std::string, const char*, const char[<Size>], const char(&)[Size] a un std::string_view
    //
    // Para solucionar esto unos de los argumentos debe tener un std::type_identity_t<T> para obligar a C++
    // que los traté como un único tipo y permitir la conversion explícita de los tipos anteriormente mencionados al std::string_view
    //
    // void TypeIdentityFunctionCase2(const std::basic_string_view<CharType, TraitType>, const std::type_identity_t<std::basic_string_view<CharType, TraitType>>)
    //
    // TypeIdentityFunctionCase2("Text", "Text");     // No se puede deducir
    // TypeIdentityFunctionCase2("Text", "Text"sv);   // No se puede deducir
    TypeIdentityFunctionCase2("Text"sv, "Text"); // OK. Si se puede deducir porque el primer argumento es: std::string_view
    // TypeIdentityFunctionCase2("Text"s, "Text"s);   // No se puede deducir
    // TypeIdentityFunctionCase2("Text"s, "Text"sv);  // No se puede deducir
    TypeIdentityFunctionCase2("Text"sv, "Text"s);  // OK. Si se puede deducir porque el primer argumento es: std::string_view
    TypeIdentityFunctionCase2("Text"sv, "Text"sv); // OK. Si se puede deducir porque el primer argumento es: std::string_view

    // En este escenario 3 tenemos que si intercambiamos el std::type_identity_t<T> hacia el primer argumento sucede lo mismo que el escenario 2.
    // Pero la deducción sólo funciona si el segundo argumento se indica explícitamente como un std::string_view.
    //
    // void TypeIdentityFunctionCase3(const std::type_identity_t<std::basic_string_view<CharType, TraitType>>, const std::basic_string_view<CharType, TraitType>)
    //
    // TypeIdentityFunctionCase3("Text", "Text");     // No se puede deducir
    TypeIdentityFunctionCase3("Text", "Text"sv); // OK. Si se puede deducir porque el segundo argumento es: std::string_view
    // TypeIdentityFunctionCase3("Text"sv, "Text");   // No se puede deducir
    // TypeIdentityFunctionCase3("Text"s, "Text"s);   // No se puede deducir
    TypeIdentityFunctionCase3("Text"s, "Text"sv); // OK. Si se puede deducir porque el segundo argumento es: std::string_view
    // TypeIdentityFunctionCase3("Text"sv, "Text"s);  // No se puede deducir
    TypeIdentityFunctionCase3("Text"sv, "Text"sv); // OK. Si se puede deducir porque el segundo argumento es: std::string_view

    // En este escenario 4 se ha usado std::type_identity_t<T> en ambos argumentos.
    // Al igual que pasaba en el escenario 1 C++ no es capaz de determinar ninguna combinación de argumentos a menos que se indique explícitamente que ambos son std::string_view.
    // Usar en ambos argumentos std::type_identity_t<T> es lo mismo que no usarlos.
    //
    // void TypeIdentityFunctionCase4(const std::type_identity_t<std::basic_string_view<CharType, TraitType>>, const std::type_identity_t<std::basic_string_view<CharType, TraitType>>)
    //
    // TypeIdentityFunctionCase4("Text", "Text");     // No se puede deducir
    // TypeIdentityFunctionCase4("Text", "Text"sv);   // No se puede deducir
    // TypeIdentityFunctionCase4("Text"sv, "Text");   // No se puede deducir
    // TypeIdentityFunctionCase4("Text"s, "Text"s);   // No se puede deducir
    // TypeIdentityFunctionCase4("Text"s, "Text"sv);  // No se puede deducir
    // TypeIdentityFunctionCase4("Text"sv, "Text"s);  // No se puede deducir
    // TypeIdentityFunctionCase4("Text"sv, "Text"sv); // No se puede deducir

    // A como se vió en los 4 escenarios anteriores. Una plantilla con un Tipo Genérico <T> con varios argumentos no es capaz de deducirse a menos que todos los argumentos sean del mismo tipo.
    // Además aunque se use std::type_identity_t<T> sólo el argumento que no esté marcado con esa clase es el que indica el tipo de los demas argumentos.
    // De los escenarios no permite combinación de tipos de forma flexible.
    //
    // Para este escenario 5 se optó por crear dicha función en 2 partes.
    // La parte de Implementación que es la función con los argumentos explícitos y la parte que transforma los argumentos.
    //
    // Para permitir cualquier tipo de argumento todos estos deben ser Tipos Genéricos puros.
    // Se puede hacer uso de conceptos para definir el comportamiento deseado.
    // Luego dentro de la función se invoca a la implementación de forma explícita evitando de esta manera el que el compilador no pueda deducir los tipos.
    //
    // template <StringViewCompatible T, StringViewCompatible U>
    // inline constexpr void TypeIdentityFunctionCase5(T&& string1, U&& string2) requires std::same_as<StringTraitsT<T>, StringTraitsT<U>>
    //
    TypeIdentityFunctionCase5(Text, "Text");           // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text", Text);           // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(Text, "Text"s);          // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"s, Text);          // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(Text, "Text"sv);         // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"sv, Text);         // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextSpan, "Text");       // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text", TextSpan);       // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextSpan, "Text"s);      // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"s, TextSpan);      // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextSpan, "Text"sv);     // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"sv, TextSpan);     // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextArray, "Text");      // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text", TextArray);      // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextArray, "Text"s);     // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"s, TextArray);     // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextArray, "Text"sv);    // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"sv, TextArray);    // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextVector, "Text");     // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text", TextVector);     // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextVector, "Text"s);    // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"s, TextVector);    // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextVector, "Text"sv);   // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"sv, TextVector);   // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(Text, TextArray);        // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextArray, Text);        // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(Text, TextVector);       // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextVector, Text);       // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(Text, TextSpan);         // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextSpan, Text);         // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextArray, TextVector);  // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextVector, TextArray);  // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextArray, TextSpan);    // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextSpan, TextArray);    // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextVector, TextSpan);   // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextSpan, TextVector);   // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text", "Text"s);        // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"s, "Text");        // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text", "Text"sv);       // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"sv, "Text");       // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(Text, Text);             // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text", "Text");         // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"s, "Text"s);       // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5("Text"sv, "Text"sv);     // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextSpan, TextSpan);     // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextArray, TextArray);   // OK. Si se puede deducir cualquier combinación de argumentos.
    TypeIdentityFunctionCase5(TextVector, TextVector); // OK. Si se puede deducir cualquier combinación de argumentos.
  }
} // namespace

TEST(StandardLibraryTests, LexicographicalCompare)
{
}

TEST(StandardLibraryTests, StringView)
{
  std::string       string_string = "Text";
  std::vector<char> string_vector{ 'T', 'e', 'x', 't', '\0' };

  [[maybe_unused]] std::string_view Text1 = "Text";
  [[maybe_unused]] std::string_view Text2 = "Text"sv;
  [[maybe_unused]] std::string_view Text3 = string_string;
  [[maybe_unused]] std::string_view Text4{ string_vector };
  [[maybe_unused]] std::string_view Text5 = (const char*)"Text";
  [[maybe_unused]] std::string_view Text6{
    std::array<char, 5>{'T', 'e', 'x', 't', '\0'}
  };
}

TEST(StandardLibraryTests, ArrayView)
{
  [[maybe_unused]] const std::string                  string      = "Text";
  [[maybe_unused]] const std::vector<char>            vector      = { std::from_range, "Text" };
  [[maybe_unused]] constexpr std::string_view         string_view = "Text";
  [[maybe_unused]] constexpr const char*              pointer     = "Text";
  [[maybe_unused]] constexpr const char               array[5]    = "Text";
  [[maybe_unused]] constexpr std::array<char, 5>      std_array   = { 'T', 'e', 'x', 't' };
  [[maybe_unused]] constexpr std::span<const char, 5> span        = "Text";

  EXPECT_TRUE(std::ranges::contiguous_range<std::string>);
  EXPECT_TRUE(std::ranges::contiguous_range<std::string_view>);
  EXPECT_TRUE(std::ranges::contiguous_range<std::ranges::subrange<const char*>>);
  EXPECT_TRUE(std::ranges::contiguous_range<const char[5]>);
  EXPECT_TRUE((std::ranges::contiguous_range<std::array<char, 5>>));
  EXPECT_TRUE(std::ranges::contiguous_range<std::vector<char>>);
  EXPECT_TRUE(std::ranges::contiguous_range<std::span<char>>);

  [[maybe_unused]] std::span<const char> span_string{ string };
  [[maybe_unused]] std::span<const char> span_string_view{ string_view };
  [[maybe_unused]] std::span<const char> span_pointer{ pointer, 4 };
  [[maybe_unused]] std::span<const char> span_array{ array };
  [[maybe_unused]] std::span<const char> span_std_array{ std_array };
  [[maybe_unused]] std::span<const char> span_vector{ vector };
  [[maybe_unused]] std::span<const char> span_span{ span };

  EXPECT_EQ(span_string.data(), string.data());
  EXPECT_EQ(span_string_view.data(), string_view.data());
  EXPECT_EQ(span_pointer.data(), pointer);
  EXPECT_EQ(span_array.data(), array);
  EXPECT_EQ(span_std_array.data(), std_array.data());
  EXPECT_EQ(span_vector.data(), vector.data());
  EXPECT_EQ(span_span.data(), span.data());
}
