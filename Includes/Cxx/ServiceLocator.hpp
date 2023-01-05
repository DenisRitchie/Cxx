#ifndef E1D945BB_547E_48B0_9B18_5B42135FBFA2
#define E1D945BB_547E_48B0_9B18_5B42135FBFA2

#include "Details/Optional.hpp"
#include "Details/Reference.hpp"
#include "Details/TypeTraits.hpp"
#include "SemanticValue.hpp"

#include <string>
#include <optional>
#include <concepts>
#include <stdexcept>
#include <typeindex>
#include <functional>
#include <unordered_map>

namespace NativeDesignPatterns
{
  class ServiceLocator
  {
    public:
      ServiceLocator()                                 = default;
      ServiceLocator(const ServiceLocator&)            = delete;
      ServiceLocator(ServiceLocator&&)                 = delete;
      ServiceLocator& operator=(const ServiceLocator&) = delete;
      ServiceLocator& operator=(ServiceLocator&&)      = delete;
      ~ServiceLocator()                                = default;

      static ServiceLocator& Default() noexcept;

      // clang-format off

      template <typename... FactoryTypes, typename... FactoryFunctions>
      requires ((Details::ServiceLocatorFactoryTraits<FactoryTypes>::IsDefaultConstructible && requires(const FactoryTypes Factory)
      {
        { Factory() } -> std::same_as<SemanticValue<typename TemplateTraits<decltype(Factory())>::ElementType>>;
      }) && ...)
      ServiceLocator& InvokeFactory(FactoryFunctions&&... Factory)
      {
        (m_Services.insert_or_assign(
          std::type_index(typeid(typename Details::ServiceLocatorFactoryTraits<FactoryTypes>::ValueType)),
          std::move(FactoryTypes()())), ...);

        (m_Services.insert_or_assign(
          std::type_index(typeid(typename Details::ServiceLocatorFactoryTraits<FactoryFunctions>::ValueType)),
          std::move(Factory())), ...);

        return *this;
      }

      // clang-format on

      template <typename ServiceType, typename ServiceValue = ServiceType, typename... Arguments>
      ServiceLocator& Register(Arguments&&... Args)
      {
        return Register<ServiceType, ServiceValue>(ServiceValue(std::forward<Arguments>(Args)...));
      }

      template <typename ServiceType, typename ServiceValue>
      ServiceLocator& Register(ServiceValue&& Value)
      {
        if constexpr ( std::is_invocable_v<ServiceValue> )
        {
          m_Services.insert_or_assign(std::type_index(typeid(ServiceType)), SemanticValue<ServiceType>(std::invoke(Value)));
        }
        else
        {
          m_Services.insert_or_assign(std::type_index(typeid(ServiceType)), SemanticValue<ServiceType>(Value));
        }

        return *this;
      }

      template <typename ServiceType, typename Self>
      auto GetService(this Self&& This) noexcept(false) //
          -> std::add_lvalue_reference_t<std::conditional_t<
              std::is_const_v<std::remove_reference_t<Self>>,
              /* case true  */ std::add_const_t<ServiceType>,
              /* case false */ ServiceType>>
      {
        if ( auto It = This.m_Services.find(std::type_index(typeid(ServiceType))); It != This.m_Services.end() )
        {
          return *std::any_cast<std::add_lvalue_reference_t<std::conditional_t< //
              std::is_const_v<std::remove_reference_t<Self>>,
              /* case true  */ std::add_const_t<SemanticValue<ServiceType>>,
              /* case false */ SemanticValue<ServiceType>>>>( //
              It->second
          );
        }

        throw std::logic_error("Unregistered Service");
      }

      template <typename ServiceType, typename Self>
      auto Resolve(this Self&& This) noexcept //
          -> std::conditional_t<
              std::is_const_v<std::remove_reference_t<Self>>,
              /* case true  */ const Optional<const Reference<const ServiceType>>,
              /* case false */ Optional<Reference<ServiceType>>>
      {
        if ( auto It = This.m_Services.find(std::type_index(typeid(ServiceType))); It != This.m_Services.end() )
        {
          return *std::any_cast<std::conditional_t< //
              std::is_const_v<std::remove_reference_t<Self>>,
              /* case true  */ const SemanticValue<ServiceType>,
              /* case false */ SemanticValue<ServiceType>>&>( //
              It->second
          );
        }

        return std::nullopt;
      }

      template <typename ServiceType>
      inline static Optional<SemanticValue<ServiceType>> Value;

      // clang-format off

      template <typename... FactoryTypes, typename ...FactoryFunctions>
      requires ((Details::ServiceLocatorFactoryTraits<FactoryTypes>::IsDefaultConstructible && requires(const FactoryTypes Factory)
      {
        { Factory() } -> std::same_as<SemanticValue<typename TemplateTraits<decltype(Factory())>::ElementType>>;
      }) && ...)
      inline static void UseFactory(FactoryFunctions &&... Factory)
      {
        ((ServiceLocator::Value<typename Details::ServiceLocatorFactoryTraits<FactoryTypes>::ValueType> = std::move(FactoryTypes()())), ...);
        ((ServiceLocator::Value<typename Details::ServiceLocatorFactoryTraits<FactoryFunctions>::ValueType> = std::move(Factory())), ...);
      }

      // clang-format on

    private:
      std::unordered_map<std::type_index, std::any> m_Services;
  };
} // namespace NativeDesignPatterns

#endif /* E1D945BB_547E_48B0_9B18_5B42135FBFA2 */
