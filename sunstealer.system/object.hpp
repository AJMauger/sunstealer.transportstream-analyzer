#pragma once

namespace ajm::system {
  class Object {
    public: explicit Object(std::string name) 
      : name(name)
    { }

    public: virtual ~Object() = default;

    public: virtual void Debug() {
      std::printf("\nObject::Debug() not implemented.");
    }

    public:std::string name;
  };
}
