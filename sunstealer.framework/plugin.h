#pragma once 

namespace ajm::framework
{
  class IPlugin {
    public: virtual ~IPlugin() = default;
    public: virtual Device *Create(ajm::framework::ILogger *logger, const std::map<std::string, std::string> parameters) = 0;
    public: virtual void Destroy() = 0;
    public: virtual std::string GetName() = 0;
  };
}
