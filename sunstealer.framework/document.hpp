#pragma once 

namespace ajm::framework {
  class IDevice;

  class Document : public ajm::system::Object {
    enum eTYPE {
      eTYPE_ISO13818_1
    };
    public: eTYPE type;

    public: explicit Document(std::string name)
      : ajm::system::Object(name) {}

    public: virtual ~Document() {
      for(std::vector<Document *>::iterator i=this->documents.begin(); i!=this->documents.end(); i++) {
        (*i)->OnRelease();
      }
    }

    public: virtual void OnRelease()=0;

    public: void Register(Document *document) {  // ajm: todo
      this->documents.push_back(document);
    }
 
    public: void Unregister(Document *document) {  // ajm: todo
      std::vector<Document *>::iterator i=std::find(this->documents.begin(), this->documents.end(), document);
      if(i!=this->documents.end()) {
        this->documents.erase(i);
      }
    }

    public: IDevice *device=nullptr;
    public: std::vector<Document *> documents;
  };
}
