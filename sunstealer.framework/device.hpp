#pragma once 

namespace ajm::framework {
  class IDevice {
    public: virtual ~IDevice() = default;
    public: enum class eSTATE {
      eSTATE_ERROR=-1,
      eSTATE_READY=0,
      eSTATE_PAUSE=1,
      eSTATE_START=2,
      eSTATE_STOP=3
    };

    public: virtual void OnState(eSTATE state)=0;
  };

  class Document;

  class Device : public ajm::system::Object, public IDevice { 
    public: explicit Device(std::string name)
      : ajm::system::Object(name) {
    }

    public: ~Device() override {
      if (this->state!=eSTATE::eSTATE_STOP) {
        this->SetState(eSTATE::eSTATE_STOP);
      }
    }

    public: virtual void OnState(eSTATE state) {
      this->state = state;
      for(std::vector<IDevice *>::iterator i=this->listeners.begin(); i!=this->listeners.end(); i++) {
        (*i)->OnState(state);
      }
    }

    public: virtual eSTATE SetState(eSTATE state) {
      this->OnState(state);
      return this->state;
    }

    public: Document *GetDocument() {
      return this->document;
    }

    public: eSTATE GetState() {
      return this->state;
    }

    public: void Register(IDevice *device) {
      this->listeners.push_back(device);
    }
 
    public: void Unregister(IDevice *device) {
      std::vector<IDevice *>::iterator i=std::find(this->listeners.begin(), this->listeners.end(), device);
      if(i!=this->listeners.end()) {
        this->listeners.erase(i);
      }
    }
    
    private: eSTATE state = eSTATE::eSTATE_READY; 

    public: Document *document=nullptr;
    public: std::vector<IDevice *> listeners;
  };
}
