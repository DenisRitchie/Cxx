// https://www.fluentcpp.com/2020/11/06/how-to-make-a-copyable-object-assignable-in-cpp/
// https://www.fluentcpp.com/2020/10/02/how-to-implement-operator-when-a-data-member-is-a-lambda/
// https://github.com/joboccara/pipes/blob/master/include/pipes/helpers/assignable.hpp
// https://github.com/ericniebler/range-v3/blob/master/include/range/v3/utility/semiregular_box.hpp

#ifndef C111AE61_3931_49D2_B028_97DAB4966DE9
#define C111AE61_3931_49D2_B028_97DAB4966DE9

#include <cassert>
#include <concepts>
#include <functional>
#include <utility>

// clang-format off

#define CPP_PP_CAT_(X, ...)  X ## __VA_ARGS__
#define CPP_PP_CAT(X, ...)   CPP_PP_CAT_(X, __VA_ARGS__)

#define CPP_PP_EVAL_(X, ARGS) X ARGS
#define CPP_PP_EVAL(X, ...) CPP_PP_EVAL_(X, (__VA_ARGS__))

#define CPP_PP_EVAL2_(X, ARGS) X ARGS
#define CPP_PP_EVAL2(X, ...) CPP_PP_EVAL2_(X, (__VA_ARGS__))

#define CPP_PP_EXPAND(...) __VA_ARGS__
#define CPP_PP_EAT(...)

#define CPP_PP_FIRST(LIST) CPP_PP_FIRST_ LIST
#define CPP_PP_FIRST_(...) __VA_ARGS__ CPP_PP_EAT

#define CPP_PP_SECOND(LIST) CPP_PP_SECOND_ LIST
#define CPP_PP_SECOND_(...) CPP_PP_EXPAND

#define CPP_PP_CHECK(...) CPP_PP_EXPAND(CPP_PP_CHECK_N(__VA_ARGS__, 0,))
#define CPP_PP_CHECK_N(x, n, ...) n
#define CPP_PP_PROBE(x) x, 1,
#define CPP_PP_PROBE_N(x, n) x, n,

// CPP_auto_fun
// Usage:
//   template <typename A, typename B>
//   auto CPP_auto_fun(foo)(A a, B b)([const]opt [noexcept(cond)]opt)opt
//   (
//       return a + b
//   )
#define CPP_auto_fun(X) X CPP_AUTO_FUN_IMPL_

/// INTERNAL ONLY
#define CPP_AUTO_FUN_IMPL_(...) (__VA_ARGS__) CPP_AUTO_FUN_RETURNS_

/// INTERNAL ONLY
#define CPP_AUTO_FUN_RETURNS_(...)                                              \
    CPP_PP_EVAL2_(                                                              \
        CPP_AUTO_FUN_SELECT_RETURNS_,                                           \
        (__VA_ARGS__,)                                                          \
    )(__VA_ARGS__)

/// INTERNAL ONLY
#define CPP_AUTO_FUN_SELECT_RETURNS_(MAYBE_CONST, ...)                          \
    CPP_PP_CAT(CPP_AUTO_FUN_RETURNS_CONST_,                                     \
        CPP_PP_EVAL(CPP_PP_CHECK, CPP_PP_CAT(                                   \
            CPP_PP_PROBE_CONST_MUTABLE_PROBE_, MAYBE_CONST)))

/// INTERNAL ONLY
#define CPP_PP_PROBE_CONST_MUTABLE_PROBE_const CPP_PP_PROBE_N(~, 1)

/// INTERNAL ONLY
#define CPP_PP_PROBE_CONST_MUTABLE_PROBE_mutable CPP_PP_PROBE_N(~, 2)

/// INTERNAL ONLY
#define CPP_PP_EAT_MUTABLE_mutable

/// INTERNAL ONLY
#define CPP_AUTO_FUN_RETURNS_CONST_2(...)                                       \
    CPP_PP_CAT(CPP_PP_EAT_MUTABLE_, __VA_ARGS__) CPP_AUTO_FUN_RETURNS_CONST_0

/// INTERNAL ONLY
#define CPP_AUTO_FUN_RETURNS_CONST_1(...)                                       \
    __VA_ARGS__ CPP_AUTO_FUN_RETURNS_CONST_0

/// INTERNAL ONLY
#define CPP_AUTO_FUN_RETURNS_CONST_0(...)                                       \
    CPP_PP_EVAL(CPP_AUTO_FUN_DECLTYPE_NOEXCEPT_,                                \
        CPP_PP_CAT(CPP_AUTO_FUN_RETURNS_, __VA_ARGS__))

/// INTERNAL ONLY
#define CPP_AUTO_FUN_RETURNS_return

#ifdef __cpp_guaranteed_copy_elision
/// INTERNAL ONLY
#define CPP_AUTO_FUN_DECLTYPE_NOEXCEPT_(...)                                    \
    noexcept(noexcept(__VA_ARGS__)) -> decltype(__VA_ARGS__)                    \
    { return (__VA_ARGS__); }

#else
/// INTERNAL ONLY
#define CPP_AUTO_FUN_DECLTYPE_NOEXCEPT_(...)                                    \
    noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) ->                   \
    decltype(__VA_ARGS__)                                                       \
    { return (__VA_ARGS__); }

#endif

// clang-format on

namespace Cxx::Helpers
{
  template <typename T>
  struct semiregular_box;

  namespace detail
  {
    struct semiregular_get
    {
        // clang-format off
        template<typename T>
        friend auto CPP_auto_fun(get)(std::type_identity_t<semiregular_box<T>> &t)
        (
            return t.get()
        )
        template<typename T>
        friend auto CPP_auto_fun(get)(std::type_identity_t<semiregular_box<T>> const &t)
        (
            return t.get()
        )
        template<typename T>
        friend auto CPP_auto_fun(get)(std::type_identity_t<semiregular_box<T>> &&t)
        (
            return std::move(t).get()
        )
        // clang-format on
    };
  } // namespace detail

  template <typename T>
  struct semiregular_box : private detail::semiregular_get
  {
    private:
      struct tag
      {
      };

      template <typename... Args>
      void construct_from(Args&&... args)
      {
        new ((void*)std::addressof(data_)) T(static_cast<Args&&>(args)...);
        engaged_ = true;
      }

      void move_assign(T&& t, std::true_type)
      {
        data_ = std::move(t);
      }

      void move_assign(T&& t, std::false_type)
      {
        reset();
        construct_from(std::move(t));
      }

      void copy_assign(const T& t, std::true_type)
      {
        data_ = t;
      }

      void copy_assign(T&& t, std::false_type)
      {
        reset();
        construct_from(t);
      }

      constexpr semiregular_box(tag, std::false_type) noexcept
      {
      }

      constexpr semiregular_box(tag, std::true_type) noexcept(std::is_nothrow_default_constructible<T>::value)
        : data_{}
        , engaged_(true)
      {
      }

      void reset()
      {
        if ( engaged_ )
        {
          data_.~T();
          engaged_ = false;
        }
      }

      union
      {
          char ch_{};
          T    data_;
      };

      bool engaged_{ false };

    public:
      constexpr semiregular_box() noexcept(std::is_nothrow_default_constructible<T>::value || !std::is_default_constructible<T>::value)
        : semiregular_box(tag{}, std::is_default_constructible<T>{})
      {
      }

      semiregular_box(semiregular_box&& that) noexcept(std::is_nothrow_move_constructible<T>::value)
      {
        if ( that.engaged_ )
          this->construct_from(std::move(that.data_));
      }

      semiregular_box(const semiregular_box& that) noexcept(std::is_nothrow_copy_constructible<T>::value)
      {
        if ( that.engaged_ )
          this->construct_from(that.data_);
      }

#if defined(__cpp_conditional_explicit) && 0 < __cpp_conditional_explicit
      template <typename U>
      requires((!std::same_as<std::remove_cvref_t<U>, semiregular_box>) and std::constructible_from<T, U>)
      explicit(!std::convertible_to<U, T>) constexpr semiregular_box(U&& u) noexcept(std::is_nothrow_constructible<T, U>::value)
        : semiregular_box(std::in_place, static_cast<U&&>(u))
      {
      }
#else
      template <typename U>
      requires((!std::same_as<std::remove_cvref_t<U>, semiregular_box>) and std::constructible_from<T, U> and (!std::convertible_to<U, T>))
      constexpr explicit semiregular_box(U&& u) noexcept(std::is_nothrow_constructible<T, U>::value)
        : semiregular_box(std::in_place, static_cast<U&&>(u))
      {
      }

      template <typename U>
      requires((!std::same_as<std::remove_cvref_t<U>, semiregular_box>) and std::constructible_from<T, U> and std::convertible_to<U, T>)
      constexpr semiregular_box(U&& u) noexcept(std::is_nothrow_constructible<T, U>::value)
        : semiregular_box(std::in_place, static_cast<U&&>(u))
      {
      }
#endif

      template <typename... Args>
      requires(std::constructible_from<T, Args...>)
      constexpr semiregular_box(std::in_place_t, Args&&... args) noexcept(std::is_nothrow_constructible<T, Args...>::value)
        : data_(static_cast<Args&&>(args)...)
        , engaged_(true)
      {
      }

      ~semiregular_box()
      {
        reset();
      }

      semiregular_box& operator=(semiregular_box&& that) noexcept(std::is_nothrow_move_constructible<T>::value && (!std::is_move_assignable<T>::value || std::is_nothrow_move_assignable<T>::value))
      {
        if ( engaged_ && that.engaged_ )
          this->move_assign(std::move(that.data_), std::is_move_assignable<T>());
        else if ( that.engaged_ )
          this->construct_from(std::move(that.data_));
        else if ( engaged_ )
          this->reset();
        return *this;
      }

      semiregular_box& operator=(const semiregular_box& that) noexcept(std::is_nothrow_copy_constructible<T>::value && (!std::is_copy_assignable<T>::value || std::is_nothrow_copy_assignable<T>::value))
      {
        if ( engaged_ && that.engaged_ )
          this->copy_assign(that.data_, std::is_copy_assignable<T>());
        else if ( that.engaged_ )
          this->construct_from(that.data_);
        else if ( engaged_ )
          this->reset();
        return *this;
      }

      constexpr T& get() & noexcept
      {
        return assert(engaged_), data_;
      }

      constexpr const T& get() const& noexcept
      {
        return assert(engaged_), data_;
      }

      constexpr T&& get() && noexcept
      {
        return assert(engaged_), std::move(data_);
      }

      const T&& get() const&& = delete;

      constexpr operator T&() & noexcept
      {
        return get();
      }

      constexpr operator const T&() const& noexcept
      {
        return get();
      }

      constexpr operator T&&() && noexcept
      {
        return std::move(get());
      }

      operator const T&&() const&& = delete;

      template <typename... Args>
      requires(std::invocable<T&, Args...>)
      constexpr decltype(auto) operator()(Args&&... args) & noexcept(std::is_nothrow_invocable_v<T&, Args...>)
      {
        return std::invoke(data_, static_cast<Args&&>(args)...);
      }

      template <typename... Args>
      requires(std::invocable<const T&, Args...>)
      constexpr decltype(auto) operator()(Args&&... args) const& noexcept(std::is_nothrow_invocable_v<const T&, Args...>)
      {
        return std::invoke(data_, static_cast<Args&&>(args)...);
      }

      template <typename... Args>
      requires(std::invocable<T, Args...>)
      constexpr decltype(auto) operator()(Args&&... args) && noexcept(std::is_nothrow_invocable_v<T, Args...>)
      {
        return std::invoke(static_cast<T&&>(data_), static_cast<Args&&>(args)...);
      }

      template <typename... Args>
      void operator()(Args&&...) const&& = delete;
  };

  template <typename T>
  struct semiregular_box<T&> : private std::reference_wrapper<T&>, private detail::semiregular_get
  {
      semiregular_box() = default;

      template <typename Arg>
      requires(std::constructible_from<std::reference_wrapper<T&>, Arg&>)
      semiregular_box(std::in_place_t, Arg& arg) noexcept
        : std::reference_wrapper<T&>(arg)
      {
      }

      using std::reference_wrapper<T&>::get;
      using std::reference_wrapper<T&>::operator T&;
      using std::reference_wrapper<T&>::operator();

#if defined(_MSC_VER)
      template <typename U>
      requires((!std::same_as<std::remove_cvref_t<U>, semiregular_box>) and std::constructible_from<std::reference_wrapper<T&>, U>)
      constexpr semiregular_box(U&& u) noexcept(std::is_nothrow_constructible<std::reference_wrapper<T&>, U>::value)
        : std::reference_wrapper<T&>{ static_cast<U&&>(u) }
      {
      }
#else
      using std::reference_wrapper<T&>::reference_wrapper;
#endif
  };

  template <typename T>
  struct semiregular_box<T&&> : private std::reference_wrapper<T&&>, private detail::semiregular_get
  {
      semiregular_box() = default;

      template <typename Arg>
      requires(std::constructible_from<std::reference_wrapper<T &&>, Arg>)
      semiregular_box(std::in_place_t, Arg&& arg) noexcept
        : std::reference_wrapper<T&&>(static_cast<Arg&&>(arg))
      {
      }

      using std::reference_wrapper<T&&>::get;
      using std::reference_wrapper<T&&>::operator T&&;
      using std::reference_wrapper<T&&>::operator();

#if defined(_MSC_VER)
      template <typename U>
      requires((!std::same_as<std::remove_cvref_t<U>, semiregular_box>) and std::constructible_from<std::reference_wrapper<T &&>, U>)
      constexpr semiregular_box(U&& u) noexcept(std::is_nothrow_constructible<std::reference_wrapper<T&&>, U>::value)
        : std::reference_wrapper<T&&>{ static_cast<U&&>(u) }
      {
      }
#else
      using std::reference_wrapper<T&&>::reference_wrapper;
#endif
  };

  template <typename T>
  using semiregular_box_t = std::conditional_t<std::semiregular<T>, T, semiregular_box<T>>;

  template <typename T, bool IsConst = false>
  using semiregular_box_ref_or_val_t = //
    std::conditional_t<
      /*Condition*/ std::semiregular<T>,
      /*True Case*/ std::conditional_t<IsConst || std::is_empty<T>::value, T, std::reference_wrapper<T>>,
      /*False Case*/ std::reference_wrapper<std::conditional_t<IsConst, const semiregular_box<T>, semiregular_box<T>>>>;

} // namespace Cxx::Helpers

#endif /* C111AE61_3931_49D2_B028_97DAB4966DE9 */
