namespace Cxx::V1
{
  template <typename CharType = char, typename TraitType = std::char_traits<CharType>, typename AllocType = std::allocator<CharType>>
  std::basic_string<CharType, TraitType, AllocType> Join(auto&& container, auto&& separator) // clang-format off
  requires requires(decltype(container) range)
  {
    { std::begin(range) } -> std::input_iterator;
    { std::end(range)   } -> std::sentinel_for<std::ranges::iterator_t<decltype(container)>>;
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
} // namespace Cxx::V1
