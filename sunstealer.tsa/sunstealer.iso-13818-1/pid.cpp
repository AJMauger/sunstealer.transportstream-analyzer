#include <algorithm>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../sunstealer.framework/logger.h"

#include "../sunstealer.system/format.hpp"
#include "../sunstealer.system/object.hpp"
#include "../sunstealer.system/system.hpp"

#include "../sunstealer.framework/document.hpp"

#include "./crc32.hpp"
#include "./descriptor.h"
#include "./packet.h"
#include "./pid.h"
#include "./section.h"
#include "./table.h"
#include "./transportstream.h"

namespace ajm::iso13818_1 {
  PID::PID(std::uint16_t pid, TransportStream *transportStream)
    : count(0), pid(pid), section(0), transportStream(transportStream), type(eTYPE::eTYPE_UNKNOWN) {

    _logger.LogDebug(std::format("PID::PID() pid: %d 0x%04x.", pid, pid).c_str());
    if(!pid) {
      this->type=eTYPE::eTYPE_SECTION;  // ajm: pat => bootstrap
    }
  }

  PID::~PID() {
    // ajm: _logger.LogDebug(std::format("PID::~PID() pid: %d 0x%04x.", pid, pid).c_str());

    if (this->section) {
      delete this->section;
    }

    this->tables.clear();
  }

  void PID::OnPacket(Packet *packet) {
    try {
      this->count++;

      const auto header=reinterpret_cast<PacketHeader *>(packet->payload);
      if(header->adaptation_field_control==0x00) { // ajm: reserved 
        return;
      }

      // ajm: adaptation_field
      if(header->adaptation_field_control>0x01) {
        this->transportStream->OnAdaptationField(packet, this);		
      }

      if(this->type==eTYPE::eTYPE_SECTION) {
        std::uint8_t i=4;
        std::uint8_t tableId=0xff;

        if(header->payload_start_indicator) {
          if(header->adaptation_field_control&0x02) { // adaptation_field()
            i+=header->adaptation_field_length+1;
          }
          i+=packet->payload[i]+1; // pointer_field()
          tableId=packet->payload[i]; 
        }

        if(!this->section) {
          this->section=new Section(this);
        }

        if(tableId!=0xff) {
          this->section->OnPacket(packet);
        }
      }

      if(this->type==eTYPE::eTYPE_PACKETIZED_ELEMENTARY_STREAM) {
        // ajm: ...
      }
    } catch(...) {
      _logger.LogException(std::format("Section::OnPacket(): unhandled exception pid: %d 0x%04x packet: %" PRIu64 ".", this->pid, this->pid, packet->index).c_str());
    }
  }
}
