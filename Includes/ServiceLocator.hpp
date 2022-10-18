#ifndef E1D945BB_547E_48B0_9B18_5B42135FBFA2
#define E1D945BB_547E_48B0_9B18_5B42135FBFA2

#include "Optional.hpp"
#include "Reference.hpp"
#include "SemanticValue.hpp"

#include <string>
#include <optional>
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

      template <typename ServiceType, typename ServiceValue = ServiceType>
      ServiceLocator& Register()
      {
        m_Services.insert_or_assign(std::type_index(typeid(ServiceType)), SemanticValue<ServiceType>(ServiceValue{}));

        return *this;
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

      template <typename ServiceType>
      const ServiceType& GetService() const
      {
        if ( auto It = m_Services.find(std::type_index(typeid(ServiceType))); It != m_Services.end() )
        {
          return *std::any_cast<const SemanticValue<ServiceType>&>(It->second);
        }

        throw std::logic_error("Unregistered Service");
      }

      template <typename ServiceType>
      ServiceType& GetService()
      {
        if ( auto It = m_Services.find(std::type_index(typeid(ServiceType))); It != m_Services.end() )
        {
          return *std::any_cast<SemanticValue<ServiceType>&>(It->second);
        }

        throw std::logic_error("Unregistered Service");
      }

      template <typename ServiceType>
      const Optional<Reference<ServiceType>> GetServiceOrDefault() const
      {
        if ( auto It = m_Services.find(std::type_index(typeid(ServiceType))); It != m_Services.end() )
        {
          return *std::any_cast<const SemanticValue<ServiceType>&>(It->second);
        }

        return std::nullopt;
      }

      template <typename ServiceType>
      Optional<Reference<ServiceType>> GetServiceOrDefault()
      {
        if ( auto It = m_Services.find(std::type_index(typeid(ServiceType))); It != m_Services.end() )
        {
          return *std::any_cast<SemanticValue<ServiceType>&>(It->second);
        }

        return std::nullopt;
      }

    private:
      std::unordered_map<std::type_index, std::any> m_Services;
  };
} // namespace NativeDesignPatterns

#endif /* E1D945BB_547E_48B0_9B18_5B42135FBFA2 */
