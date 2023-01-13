#ifndef D3D42556_E9FB_4FD2_893B_B63630BF057E
#define D3D42556_E9FB_4FD2_893B_B63630BF057E

namespace Cxx
{
  template <typename Derived>
  struct Crtp
  {
      [[nodiscard]] constexpr Derived& Underlying() noexcept
      {
        return static_cast<Derived&>(*this);
      }

      [[nodiscard]] constexpr const Derived& Underlying() const noexcept
      {
        return static_cast<const Derived&>(*this);
      }
  };
} // namespace Cxx

#endif /* D3D42556_E9FB_4FD2_893B_B63630BF057E */
