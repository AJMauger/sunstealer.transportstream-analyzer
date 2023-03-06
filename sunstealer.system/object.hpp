#pragma once

namespace ajm::system {
  class Object {
    public: Object(const std::string name) {
      this->name = name;
    }

    public: virtual ~Object() {} // ajm: = default; // ajm: ?

    public: virtual void Debug() {
      std::printf("\nObject::Debug() not implemented.");
    }

    public:std::string name;
  };
}
