#pragma once 

namespace ajm::framework
{
  class ILogger {
    public: virtual ~ILogger() = default;
    public: virtual void Close()=0;
    public: virtual void LogDebug(const char *message)=0;
    public: virtual void LogInformation(const char *message)=0;
    public: virtual void LogWarning(const char *message)=0;
    public: virtual void LogError(const char *message)=0;
    public: virtual void LogException(const char *message)=0;
    public: virtual void Open()=0;
  };
}
