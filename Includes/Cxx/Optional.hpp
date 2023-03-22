#ifndef BEAE9345_441A_49A7_AB8A_28C3C1F463AD
#define BEAE9345_441A_49A7_AB8A_28C3C1F463AD

#include "FunctionTraits.hpp"
#include "Platform.hpp"
#include "Utility.hpp"
#include <optional>

namespace Cxx
{
  template <typename Type>
  struct Optional : public std::optional<Type>
  {
      using base_type = std::optional<Type>;
      using std::optional<Type>::optional;
      using std::optional<Type>::operator=;

#if defined(__cpp_explicit_this_parameter) and defined(__cpp_multidimensional_subscript)
      template <typename Self, std::integral... Index>
      constexpr decltype(auto) operator[](this Self&& self, const Index... indices)
      requires requires { self->operator[](indices...); }
      {
        return self->operator[](indices...);
      }
#else
      constexpr decltype(auto) operator[](const int32_t index) &
        requires requires { (*this)->operator[](index); }
      {
        return (*this)->operator[](index);
      }

      constexpr decltype(auto) operator[](const int32_t index) const&
      requires requires { (*this)->operator[](index); }
      {
        return (*this)->operator[](index);
      }

      constexpr decltype(auto) operator[](const int32_t index) &&
        requires requires { (*this)->operator[](index); }
      {
        return std::move((*this)->operator[](index));
      }

      constexpr decltype(auto) operator[](const int32_t index) const&&
      requires requires { (*this)->operator[](index); }
      {
        return std::move((*this)->operator[](index));
      }
#endif

#ifdef __cpp_explicit_this_parameter
      template <typename Self>
      constexpr decltype(auto) operator*(this Self&& self)
      {
        if ( not self.has_value() )
          throw std::bad_optional_access{};

        return Cxx::Utilities::deep_smart_pointer_dereference(self.base_type::operator->());
      }
#else
      constexpr decltype(auto) operator*() &
      {
        if ( not base_type::has_value() )
          throw std::bad_optional_access{};

        return Cxx::Utilities::deep_smart_pointer_dereference(base_type::operator->());
      }

      constexpr decltype(auto) operator*() const&
      {
        if ( not base_type::has_value() )
          throw std::bad_optional_access{};

        return Cxx::Utilities::deep_smart_pointer_dereference(base_type::operator->());
      }

      constexpr decltype(auto) operator*() &&
      {
        if ( not base_type::has_value() )
          throw std::bad_optional_access{};

        return std::move(Cxx::Utilities::deep_smart_pointer_dereference(base_type::operator->()));
      }

      constexpr decltype(auto) operator*() const&&
      {
        if ( not base_type::has_value() )
          throw std::bad_optional_access{};

        return std::move(Cxx::Utilities::deep_smart_pointer_dereference(base_type::operator->()));
      }
#endif

#ifdef __cpp_explicit_this_parameter
      template <typename Self>
      constexpr decltype(auto) operator->(this Self&& self)
      {
        using Cxx::Utilities::deep_raw_pointer_dereference;

        if ( not self.has_value() )
          throw std::bad_optional_access{};

        if constexpr ( requires { deep_raw_pointer_dereference(self.base_type::operator->()).operator->(); } )
        {
          return deep_raw_pointer_dereference(self.base_type::operator->());
        }
        else
        {
          return &deep_raw_pointer_dereference(self.base_type::operator->());
        }
      }
#else
      constexpr decltype(auto) operator->()
      {
        using Cxx::Utilities::deep_raw_pointer_dereference;

        if ( not base_type::has_value() )
          throw std::bad_optional_access{};

        if constexpr ( requires { deep_raw_pointer_dereference(base_type::operator->()).operator->(); } )
        {
          return deep_raw_pointer_dereference(base_type::operator->());
        }
        else
        {
          return &deep_raw_pointer_dereference(base_type::operator->());
        }
      }

      constexpr decltype(auto) operator->() const
      {
        using Cxx::Utilities::deep_raw_pointer_dereference;

        if ( not base_type::has_value() )
          throw std::bad_optional_access{};

        if constexpr ( requires { deep_raw_pointer_dereference(base_type::operator->()).operator->(); } )
        {
          return deep_raw_pointer_dereference(base_type::operator->());
        }
        else
        {
          return &deep_raw_pointer_dereference(base_type::operator->());
        }
      }
#endif
  };
} // namespace Cxx

#endif /* BEAE9345_441A_49A7_AB8A_28C3C1F463AD */
