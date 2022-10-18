#include "ServiceLocator.hpp"

#include <atomic>
#include <mutex>

namespace NativeDesignPatterns
{
  struct ThreadSafeServiceLocator
  {
      static ServiceLocator& GetInstance()
      {
        std::call_once(CreateFlag, [=] { Instance = std::make_unique<ServiceLocator>(); });
        return *Instance;
      }

      static std::unique_ptr<ServiceLocator> Instance;
      static std::once_flag                  CreateFlag;
  };

  ServiceLocator& ServiceLocator::Default() noexcept
  {
    return ThreadSafeServiceLocator::GetInstance();
  }

  std::unique_ptr<ServiceLocator> ThreadSafeServiceLocator::Instance;
  std::once_flag                  ThreadSafeServiceLocator::CreateFlag;
} // namespace NativeDesignPatterns
