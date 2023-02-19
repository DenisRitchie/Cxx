namespace Cxx::Algorithms::V1
{
  template <typename CharType = char, typename TraitType = std::char_traits<CharType>, typename AllocType = std::allocator<CharType>>
  std::basic_string<CharType, TraitType, AllocType> Join(std::ranges::input_range auto&& container, auto&& separator)
  {
    using ContainerType = decltype(container);
    using SeparatorType = decltype(separator);

    std::basic_ostringstream<CharType, TraitType, AllocType> output;

    std::ranges::copy( //
      std::ranges::begin(std::forward<ContainerType>(container)),
      std::ranges::end(std::forward<ContainerType>(container)),
      Cxx::DesignPatterns::MakeOstreamJoiner(output, std::forward<SeparatorType>(separator))
    );

    return output.str();
  };
} // namespace Cxx::Algorithms::V1
