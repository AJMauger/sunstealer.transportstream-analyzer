#pragma once

namespace ajm::iso13818_1_file {
  class Plugin : public ajm::framework::Device, public ajm::iso13818_1::TransportStream {
    private: explicit Plugin(const std::map<std::string, std::string> parameters)
      : ajm::framework::Device(::GetName()), ajm::iso13818_1::TransportStream(::GetName()), parameters(parameters) {
      if (!_plugin) {
        _plugin = this;        
        this->device=this;
        this->document=this;
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
        _logger.LogDebug(std::format("ajm::iso13818_1_file::TransportStream::SetState(): %d", state).c_str());
        switch(state) {
          case ajm::framework::IDevice::eSTATE::eSTATE_START:
            if (_plugin->GetState() == ajm::framework::IDevice::eSTATE::eSTATE_READY) {
              std::string uri=(*this->parameters.find("uri")).second;
              std::fstream fs(uri.c_str(), std::ios::binary|std::ios::in);
              if (!fs.bad()) {
                Device::SetState(IDevice::eSTATE::eSTATE_START);
                std::int32_t read = 188 * 55775;
                std::int32_t size = read; 
                std::future<std::int32_t> fa = std::async(std::launch::async, [&] () {
                  try {
                    std::uint64_t index=0;
                    std::unique_ptr<std::uint8_t> buffer(new std::uint8_t[size]);
                    while (this->GetState()!=ajm::framework::IDevice::eSTATE::eSTATE_STOP && read == size) {
                      if (this->GetState()==ajm::framework::IDevice::eSTATE::eSTATE_START) {
                        read=fs.readsome(reinterpret_cast<char *>(buffer.get()), size);
                        std::int32_t packets=read/188;
                        for (std::int32_t i=0; i<packets; i++) {
                          ajm::iso13818_1::Packet *packet=new ajm::iso13818_1::Packet(&buffer.get()[i*188], index);                          
                          std::uint16_t pid=_GET_LEPID(&packet->payload[1]);
                          if (!(packet->index%1000000)) {
                            _logger.LogDebug(std::format("ajm::iso13818_1_file::packet.index: %d.", packet->index).c_str());
                          }
                          if (!this->pids[pid]) {
                            this->pids[pid]=new ajm::iso13818_1::PID(pid, this);
                            this->OnPID(this->pids[pid]);
                          }
                          this->pids[pid]->OnPacket(packet);
                          index++;
                          // ajm: this->lock.lock();
                          // ajm: this->packets.push_front(packet);
                          // ajm: this->lock.unlock();
                          delete packet;                          
                        }
                      }
                    }
                  } catch(...) {
                    _logger.LogException("iso-13818-1::file::Device::Start(): unhandled exception.");
                  }
                  return 0;
                });
                fa.get();
                fs.close();
                Device::SetState(ajm::framework::IDevice::eSTATE::eSTATE_STOP);
              }
            }
          break;
          default:
            return state;
          break; 
        }
        return ajm::framework::IDevice::eSTATE::eSTATE_STOP;
      } catch(...) {
        _logger.LogException("Plugin::Start(): unhandled exception.");
      }
      return ajm::framework::IDevice::eSTATE::eSTATE_ERROR;
    }

    public: void OnRelease() {
      _logger.LogDebug("iso-13818-1::file::Device::OnRelease().");
    }

    public: static Plugin *_plugin;

    private: std::mutex lock;
    private: std::deque<ajm::iso13818_1::Packet *> packets;
    private: std::map<std::string, std::string> parameters;
  };
}
