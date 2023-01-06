#ifndef F82E2413_03CB_430C_91DF_3D3AA19163D9
#define F82E2413_03CB_430C_91DF_3D3AA19163D9

#include <type_traits>

namespace Cxx::DetectionIdiom
{

#ifdef __GNUC__
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#endif

  struct Nonesuch
  {
      Nonesuch()                      = delete;
      ~Nonesuch()                     = delete;
      Nonesuch(const Nonesuch&)       = delete;
      void operator=(const Nonesuch&) = delete;
  };

#ifdef __GNUC__
# pragma GCC diagnostic pop
#endif

  namespace Detail
  {
    template <typename Default, typename AlwaysVoid, template <typename...> class Trait, typename... Args>
    struct Detector
    {
        using ValueType = std::false_type;
        using Type      = Default;
    };

    template <typename Default, template <typename...> class Trait, typename... Args>
    struct Detector<Default, std::void_t<Trait<Args...>>, Trait, Args...>
    {
        using ValueType = std::true_type;
        using Type      = Trait<Args...>;
    };
  } // namespace Detail

  template <template <typename...> class Trait, typename... Args>
  using IsDetected = typename Detail::Detector<Nonesuch, void, Trait, Args...>::ValueType;

  template <template <typename...> class Trait, typename... Args>
  using DetectedType = typename Detail::Detector<Nonesuch, void, Trait, Args...>::Type;

  template <typename Default, template <typename...> class Trait, typename... Args>
  using DetectedOr = Detail::Detector<Default, void, Trait, Args...>;

  template <template <class...> class Trait, class... Args>
  inline constexpr bool IsDetectedV = IsDetected<Trait, Args...>::value;

  template <class Default, template <class...> class Trait, class... Args>
  using DetectedOrType = typename DetectedOr<Default, Trait, Args...>::Type;

  template <class Expected, template <class...> class Trait, class... Args>
  using IsDetectedExact = std::is_same<Expected, DetectedType<Trait, Args...>>;

  template <class Expected, template <class...> class Trait, class... Args>
  inline constexpr bool IsDetectedExactV = IsDetectedExact<Expected, Trait, Args...>::value;

  template <class To, template <class...> class Trait, class... Args>
  using IsDetectedConvertible = std::is_convertible<DetectedType<Trait, Args...>, To>;

  template <class To, template <class...> class Trait, class... Args>
  inline constexpr bool IsDetectedConvertibleV = IsDetectedConvertible<To, Trait, Args...>::value;
} // namespace Cxx::DetectionIdiom

#endif /* F82E2413_03CB_430C_91DF_3D3AA19163D9 */
