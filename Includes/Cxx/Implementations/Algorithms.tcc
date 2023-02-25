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

  template < // clang-format off
    std::input_iterator          LeftFirst,
    std::sentinel_for<LeftFirst> LeftLast,
    std::input_iterator           RightFirst,
    std::sentinel_for<RightFirst> RightLast,
    std::invocable<std::iter_value_t<LeftFirst>>  LeftProjection,
    std::invocable<std::iter_value_t<RightFirst>> RightProjection,
    std::invocable<projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>> CompareThreeWay>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(
    LeftFirst&& left_first, LeftLast&& left_last, RightFirst&& right_first, RightLast&& right_last,
    size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection,
    CompareThreeWay&& compare_three_way_order_fallback
  ) const noexcept(noexcept(std::invoke(
      std::forward<CompareThreeWay>(compare_three_way_order_fallback),
           std::invoke(std::forward<LeftProjection>(left_projection), *left_first),
           std::invoke(std::forward<RightProjection>(right_projection), *right_first)
    ))) // clang-format on
    -> std::invoke_result_t<CompareThreeWay, projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>>
  {
    using compare_three_way_t = std::invoke_result_t<CompareThreeWay, projected_t<LeftFirst, LeftProjection>, projected_t<RightFirst, RightProjection>>;

    // clang-format off
    for ( ; number_of_items_to_compare; --number_of_items_to_compare, left_first = std::ranges::next(left_first), right_first = std::ranges::next(right_first) )
    {
      if ( left_first == left_last ) {
        return right_first == right_last ? compare_three_way_t::equivalent : compare_three_way_t::less;
      }

      if ( right_first == right_last ) {
        return compare_three_way_t::greater;
      }

      const auto compare_result = std::invoke(
        std::forward<CompareThreeWay>(compare_three_way_order_fallback),
          std::invoke(std::forward<LeftProjection>(left_projection), *left_first),
          std::invoke(std::forward<RightProjection>(right_projection), *right_first)
      );

      if ( std::is_neq(compare_result) ) {
        return compare_result;
      }
    }
    // clang-format on

    return compare_three_way_t::equivalent;
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), DefaultSize);
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), number_of_items_to_compare, CompareThreeWayOrderFallback);
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>, std::ranges::range_value_t<RightRange>> CompareThreeWay>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, CompareThreeWay&& compare_three_way_order_fallback) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), number_of_items_to_compare, Identity, Identity, std::forward<CompareThreeWay>(compare_three_way_order_fallback));
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, LeftProjection>, projected_t<std::ranges::iterator_t<RightRange>, RightProjection>> CompareThreeWay>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept
  {
    return this->operator()(std::ranges::begin(std::forward<LeftRange>(left_range)), std::ranges::end(std::forward<LeftRange>(left_range)), std::ranges::begin(std::forward<RightRange>(right_range)), std::ranges::end(std::forward<RightRange>(right_range)), number_of_items_to_compare, std::forward<LeftProjection>(left_projection), std::forward<RightProjection>(right_projection), std::forward<CompareThreeWay>(compare_three_way_order_fallback));
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>, std::ranges::range_value_t<RightRange>> CompareThreeWay>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, CompareThreeWay&& compare_three_way_order_fallback) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), DefaultSize, Identity, Identity, std::forward<CompareThreeWay>(compare_three_way_order_fallback));
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, LeftProjection&& left_projection, RightProjection&& right_projection) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), DefaultSize, std::forward<LeftProjection>(left_projection), std::forward<RightProjection>(right_projection), CompareThreeWayOrderFallback);
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, LeftProjection>, projected_t<std::ranges::iterator_t<RightRange>, RightProjection>> CompareThreeWay>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, LeftProjection&& left_projection, RightProjection&& right_projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), DefaultSize, std::forward<LeftProjection>(left_projection), std::forward<RightProjection>(right_projection), std::forward<CompareThreeWay>(compare_three_way_order_fallback));
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, std::invocable<std::ranges::range_value_t<LeftRange>> LeftProjection, std::invocable<std::ranges::range_value_t<RightRange>> RightProjection>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, LeftProjection&& left_projection, RightProjection&& right_projection) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), number_of_items_to_compare, std::forward<LeftProjection>(left_projection), std::forward<RightProjection>(right_projection), CompareThreeWayOrderFallback);
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection>
  requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, Projection&& projection) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), DefaultSize, std::forward<Projection>(projection), std::forward<Projection>(projection), CompareThreeWayOrderFallback);
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, Projection>, projected_t<std::ranges::iterator_t<RightRange>, Projection>> CompareThreeWay>
  requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, Projection&& projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), DefaultSize, std::forward<Projection>(projection), std::forward<Projection>(projection), std::forward<CompareThreeWay>(compare_three_way_order_fallback));
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection>
  requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, Projection&& projection) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), number_of_items_to_compare, std::forward<Projection>(projection), std::forward<Projection>(projection), CompareThreeWayOrderFallback);
  }

  template <std::ranges::input_range LeftRange, std::ranges::input_range RightRange, typename Projection, std::invocable<projected_t<std::ranges::iterator_t<LeftRange>, Projection>, projected_t<std::ranges::iterator_t<RightRange>, Projection>> CompareThreeWay>
  requires std::invocable<Projection, std::ranges::range_value_t<LeftRange>> and std::invocable<Projection, std::ranges::range_value_t<RightRange>>
  constexpr auto Details::FunctionObjects::RangeCompare::operator()(LeftRange&& left_range, RightRange&& right_range, const size_t number_of_items_to_compare, Projection&& projection, CompareThreeWay&& compare_three_way_order_fallback) const noexcept
  {
    return this->operator()(std::forward<LeftRange>(left_range), std::forward<RightRange>(right_range), number_of_items_to_compare, std::forward<Projection>(projection), std::forward<Projection>(projection), std::forward<CompareThreeWay>(compare_three_way_order_fallback));
  }
} // namespace Cxx::Algorithms::V1
