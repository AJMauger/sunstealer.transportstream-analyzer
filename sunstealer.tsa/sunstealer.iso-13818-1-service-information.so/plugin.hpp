#pragma once

namespace ajm::iso13818_1_serviceinformation
{
  class Plugin : public ajm::framework::Device, public ajm::iso13818_1::TransportStream 
  {
    private: explicit Plugin(const std::map<std::string, std::string> parameters)
      : ajm::framework::Device(::GetName()), ajm::iso13818_1::TransportStream(::GetName()), parameters(parameters) {
      if (!_plugin) {
        _plugin = this;
        
        this->device=this;
        this->document=this;
        std::memset(this->pids, 0x0, sizeof(this->pids));
      }
    }

    public: virtual ~Plugin() {
      this->lock.lock();
      for(std::deque<ajm::iso13818_1::Packet *>::iterator i=this->packets.begin(); i!=this->packets.end(); i++) {
        delete (*i);
      }
      this->packets.clear();
      this->lock.unlock();
    }

    public: static Plugin *GetInstance(const std::map<std::string, std::string> parameters) {
      if (!_plugin) {
        _plugin = new Plugin(parameters);
      }
      return _plugin;
    }

    // ajm: IDevice
    public: ajm::framework::IDevice::eSTATE SetState(ajm::framework::IDevice::eSTATE state) {
      try {
        _logger.LogDebug(std::format("ajm::iso13818_1_service-information::TransportStream::SetState(): %d", state).c_str());
        Device::SetState(IDevice::eSTATE::eSTATE_START);
      } catch(...) {
        _logger.LogException("Plugin::Start(): unhandled exception.");
      }
      return ajm::framework::IDevice::eSTATE::eSTATE_ERROR;
    }

    // ajm: TransportStream : Document
    public: void OnPID(ajm::iso13818_1::PID *pid) {
      if(!this->pids[pid->pid]) {
        this->pids[pid->pid]=pid;
      }
    }

    public: void OnRelease() {}

    public: static Plugin *_plugin;

    private: std::deque<ajm::iso13818_1::Packet *> packets;
    private: std::map<std::string, std::string> parameters;
    private: std::mutex lock;
    private: ajm::iso13818_1::PID* pids[0x2000];
  };
}
