#pragma once

namespace ajm::transportstreamanalyzer
{
  class Plugin {
    // ajm: ---------------------------------------------------------------------------------------
    public: ajm::framework::Device *(*create)(ajm::framework::ILogger *logger, std::map<std::string, std::string> parameters);
    public: void (*destroy)();
    public: void *handle=nullptr;

    public: ajm::framework::Device *device=nullptr;
    public: ajm::iso13818_1::TransportStream *transportStream=nullptr;
  };

  class TransportStreamAnalyzer : public ajm::framework::IDevice, public ajm::iso13818_1::TransportStream {
    // ajm: ---------------------------------------------------------------------------------------
    public: explicit TransportStreamAnalyzer(ajm::framework::ILogger *logger) 
      : ajm::iso13818_1::TransportStream("TransportStreamAnalyzer"), logger(logger) {}

    // ajm: ---------------------------------------------------------------------------------------
    public: void Analyze(std::string uri) {
      try {
        std::vector<Plugin *> plugins;
        
        // ajm: information consumer, controlled device
        std::map<std::string, std::shared_ptr<Plugin>>::iterator i=this->plugins.find("iso-13818-1::service-information");
        if (i!=this->plugins.end()) {
          Plugin *plugin=(*i).second.get();
          std::map<std::string, std::string> parameters;
          // ajm: 
          plugin->device=plugin->create(this->logger, parameters);
          plugin->transportStream=dynamic_cast<ajm::iso13818_1::TransportStream *>(plugin->device->GetDocument()); 
          // ajm: plugin->transportStream->Register(this);
          plugin->device->SetState(ajm::framework::IDevice::eSTATE::eSTATE_START);
          plugins.push_back(plugin);
        } else {
          this->logger->LogError("TransportStreamAnalyzer::Analyze(): plugin: iso-13818-1::service-information not found.");
        }        

        // ajm: file producer, controlling device
        i=this->plugins.find("iso-13818-1::file");
        if (i!=this->plugins.end()) {
          this->plugin=(*i).second.get();
          std::map<std::string, std::string> parameters;
          parameters.insert(std::pair<std::string, std::string>("uri", uri));
          this->plugin->device=plugin->create(this->logger, parameters);
          this->plugin->device->Register(this); // ajm: listen to controller
          this->plugin->transportStream=dynamic_cast<ajm::iso13818_1::TransportStream *>(plugin->device->GetDocument());
          // this->plugin->transportStream->Register(this);
          for(std::vector<Plugin *>::iterator j=plugins.begin(); j!=plugins.end(); j++) {
            this->plugin->transportStream->Register((*j)->transportStream);
          }
          this->plugin->device->SetState(ajm::framework::IDevice::eSTATE::eSTATE_START);  // ajm: blocking
        } else {
          this->logger->LogError("TransportStreamAnalyzer::Analyze(): plugin iso-13818-1::file not found.");
        }
        this->UnloadPlugins();
        plugins.clear();
      } catch(...) {
        this->logger->LogException("TransportStreamAnalyzer::Analyze(): unhandled exception");
      }
    }

    // ajm: ---------------------------------------------------------------------------------------
    public: void OnRelease() {}

    // ajm: ---------------------------------------------------------------------------------------
    public: void OnState(ajm::framework::IDevice::eSTATE state) {
      switch(state) {
        case ajm::framework::IDevice::eSTATE::eSTATE_ERROR:
        case ajm::framework::IDevice::eSTATE::eSTATE_STOP:
          for(std::map<std::string, std::shared_ptr<Plugin>>::iterator i=this->plugins.begin(); i!=plugins.end(); i++) {
            if ((*i).second.get()!=this->plugin) {
              this->logger->LogDebug(std::format("TransportStreamAnalyzer::OnState()[%s]: %d", (*i).first.c_str(), state).c_str());
              this->plugin->transportStream->Unregister((*i).second->transportStream);
            }
          }
        break;
      }
      this->logger->LogDebug("TransportStreamAnalyzer::OnState() exit");
    }

    // ajm: ---------------------------------------------------------------------------------------
    void LoadPlugins() {
      try {
        this->logger->LogDebug("TransportStreamAnalyzer::LoadPlugins()");
        for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(".")) {
          std::string uri=file.path();
          if(uri.ends_with(".so")) {
            this->LoadPlugin(uri);
          }
        }
      } catch(...) {
        this->logger->LogException("TransportStreamAnalyzer::LoadPlugins(): unhandled exception");
      }
    }

    // ajm: ---------------------------------------------------------------------------------------
    void LoadPlugin(std::string uri) {
      try {
        void *handle = dlopen(uri.c_str(), RTLD_LOCAL | RTLD_LAZY);
        if (handle) {
          std::string (*getname)() = (std::string (*)()) dlsym(handle, "GetName");
          if (getname) {
            std::string name = getname();
            this->logger->LogInformation(std::format("TransportStreamAnalyzer::LoadPlugin(): %s.", name.c_str()).c_str());
            auto (*create)(ajm::framework::ILogger *logger, std::map<std::string, std::string> parameters)
              = (ajm::framework::Device * (*)(ajm::framework::ILogger *logger, std::map<std::string, std::string> parameters)) dlsym(handle, "Create");
            auto (*destroy)() = (void (*)()) dlsym(handle, "Destroy");

            auto plugin=std::make_shared<Plugin>();
            plugin->create=create;
            plugin->destroy=destroy;
            plugin->handle=handle;
            this->plugins.insert(std::pair(name, plugin));
          } else {
            dlclose(handle);
          }
        } else {
          logger->LogError(std::format("TransportStreamAnalyzer::LoadPlugin(): %s - %s.", uri.c_str(), dlerror()).c_str());
        }
      } catch(...) {
        this->logger->LogException("TransportStreamAnalyzer::LoadPlugin(): unhandled exception");
      }
    }

    // ajm: ---------------------------------------------------------------------------------------
    void UnloadPlugins() {
      try {
        this->logger->LogDebug("TransportStreamAnalyzer::UnloadPlugins()");
        for (std::map<std::string, std::shared_ptr<Plugin>>::iterator i=this->plugins.begin(); i!=this->plugins.end(); i++) {
          this->logger->LogDebug(std::format("TransportStreamAnalyzer::UnloadPlugins(%s)", (*i).first.c_str()).c_str());
          Plugin *plugin=(*i).second.get();
          plugin->destroy();
          dlclose(plugin->handle);
          // ajm: delete (*i).second;
        }
        this->plugins.clear();
      } catch(...) {
        this->logger->LogException("TransportStreamAnalyzer::UnloadPlugin(): unhandled exception");
      }
    }

    public: ajm::framework::ILogger *logger=nullptr;
    public: Plugin *plugin=nullptr;
    public: std::map<std::string, std::shared_ptr<Plugin> > plugins;
  };
}