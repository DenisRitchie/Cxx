#ifndef D3012D70_BE77_4E7B_AAAE_6F2F76CC2D2A
#define D3012D70_BE77_4E7B_AAAE_6F2F76CC2D2A

#include <stdexcept>

namespace Cxx
{
  class UnstableIteratorException : public std::runtime_error
  {
    public:
      using std::runtime_error::runtime_error;

      UnstableIteratorException(const std::error_code error_code) noexcept
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

#endif /* D3012D70_BE77_4E7B_AAAE_6F2F76CC2D2A */
