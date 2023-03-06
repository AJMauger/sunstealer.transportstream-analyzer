#include <cstddef>
#include <cstdint>
#include <cstring>

#include "./packet.h"

namespace ajm::iso13818_1 {
  Packet::Packet(std::uint8_t *packet, std::int64_t index)
    : payload(0), index(index) {
    this->payload=new std::int8_t[188];
    std::memcpy(this->payload, packet, 188);
  }

  Packet::~Packet() {
    delete[] this->payload;
  }
}