#pragma once

#define _logger (*ajm::system::System::_system->logger)

namespace ajm::system {
  class System {
    public: System(ajm::framework::ILogger *logger) {
        if(!_system) {
          _system=this;
          this->logger=logger;
        }
      }

    public: virtual ~System() {
      _system=nullptr;
    }

    public: static System *GetInstance(ajm::framework::ILogger *logger) {
      if (_system) {
        return _system;
      }
      _system = new System(logger); 
      return _system;
    }

    public: ajm::framework::ILogger *logger;
    public: static System *_system;
  };
}
