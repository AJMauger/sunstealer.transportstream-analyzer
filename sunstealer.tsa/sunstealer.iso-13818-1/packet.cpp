#include <cstddef>
#include <cstdint>
#include <cstring>

#include "./packet.h"

namespace ajm::iso13818_1 {
  Packet::Packet(const std::uint8_t *packet, std::int64_t index)
    : index(index) {

    std::memcpy(this->payload, packet, 188);
  }
}