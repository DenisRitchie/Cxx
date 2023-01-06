#ifndef B3F8C498_3BB4_4FB7_A932_438C43F6CF2B
#define B3F8C498_3BB4_4FB7_A932_438C43F6CF2B

#include <stdexcept>

namespace Cxx
{
  class IOException : public std::runtime_error
  {
    public:
      using std::runtime_error::runtime_error;

      IOException(const std::error_code error_code) noexcept
        : std::runtime_error(error_code.message())
      {
        m_ErrorCode = error_code;
      }

      const std::error_code& ErrorCode() const noexcept
      {
        return m_ErrorCode;
      }

    private:
      std::error_code m_ErrorCode;
  };
} // namespace Cxx

#endif /* B3F8C498_3BB4_4FB7_A932_438C43F6CF2B */
