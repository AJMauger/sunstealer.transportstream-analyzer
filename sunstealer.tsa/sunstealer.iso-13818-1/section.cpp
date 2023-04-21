#include <algorithm>
#include <assert.h>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../sunstealer.framework/logger.h"

#include "../sunstealer.system/defines.h"
#include "../sunstealer.system/format.hpp"
#include "../sunstealer.system/object.hpp"
#include "../sunstealer.system/system.hpp"

#include "../sunstealer.framework/device.hpp"
#include "../sunstealer.framework/document.hpp"

#include "./crc32.hpp"
#include "./descriptor.h"
#include "./packet.h"
#include "./pid.h"
#include "./section.h"
#include "./table.h"
#include "./transportstream.h"

#include "./cat.h"
#include "./pat.h"
#include "./pmt.h"

namespace ajm::iso13818_1 {
  Section::Section(PID *pid)
    : continuityCounter(0), crc(0xffffffff), length(0), firstPacketIndex(0), lastPacketIndex(0), payloadLength(0), pid(pid) {}

  // ajm: -----------------------------------------------------------------------------------------
  void Section::OnPacket(Packet *packet) {
    try {
      // ajm: _logger.LogDebug("Section::OnPacket()");
      const auto packetHeader=reinterpret_cast<PacketHeader *>(packet->payload);

      // ajm: sync. payload_start_indicator
      if(packetHeader->payload_start_indicator) {
        this->continuityCounter=packetHeader->continuity_counter;
      } else if(this->continuityCounter==0xff) {
        return; // not sync.
      } else {
        switch(packetHeader->adaptation_field_control) {
          case 0x01:	// ajm: payload
          case 0x03:	// ajm: adaptation field + payload
            this->continuityCounter=(this->continuityCounter+1)%16;				
          break;
          default:	// ajm: no payload
            return;
        }
      }

      // ajm: continuity
      if(packetHeader->continuity_counter!=this->continuityCounter && (packetHeader->adaptation_field_control&0x01) && !packetHeader->discontinuity_indicator) {
        _logger.LogError(std::format("section discarded on packet discontinuity PID: %d packet: %" PRIu64 ".", this->pid->pid, packet->index).c_str());
        if(!packetHeader->payload_start_indicator) {
          this->continuityCounter=0xff;
          return;
        }
      }

      // ajm: payload
      std::uint8_t payloadLength=188-4;
      if(packetHeader->adaptation_field_control>0x01) {
        payloadLength-=(packetHeader->adaptation_field_length+1);
      }

      if(packetHeader->payload_start_indicator) {
        std::uint8_t pointerField=packet->payload[188-payloadLength];
        payloadLength--;

        if(this->length) {  // previous section
          if(pointerField) {
            memcpy(&this->payload[this->payloadLength], &packet->payload[188-payloadLength], pointerField);
          }
          this->crc=this->crc32.Calc(this->payload, (std::uint32_t)this->length+3);
          if(this->crc) {	// ajm: header +3
            this->Debug();
            _logger.LogError(std::format("Section::OnPacket(PID: %d): Section discarded: bad CRC", this->pid->pid).c_str());
          } else {
            OnSection();
          }
        }

        payloadLength-=pointerField;

        this->payloadLength=0;
        std::memset(this->payload, 0xff, SECTION_LENGTH);

        auto sectionHeader=reinterpret_cast<SectionHeader1 *>(&packet->payload[188-payloadLength]);

        /* ajm: Table *table=0;
        std::map<std::uint8_t, Table *>::iterator i=this->pid->tables.find(sectionHeader->table_id);	
        if(i!=this->pid->tables.end()) {
          if ((*i).second->versionNumber==sectionHeader->version_number) {
            return;
          }
        }*/

        this->firstPacketIndex=packet->index;
        this->length=((std::uint16_t)sectionHeader->length1<<8)|sectionHeader->length2;
      }	 else {
        this->lastPacketIndex=packet->index;
      }

      if(this->firstPacketIndex) {
        assert(188-payloadLength>0);

        memcpy(&this->payload[this->payloadLength], &packet->payload[188-payloadLength], payloadLength);			
        this->payloadLength+=payloadLength;

        assert(this->payloadLength<=this->length+188);

        this->lastPacketIndex=packet->index+1;
        if(this->payloadLength>=this->length+3) {
          this->crc=this->crc32.Calc(this->payload, (std::uint32_t)this->length+3);
          if(this->crc) {	// ajm: +3
            this->Debug();
            _logger.LogError(std::format("Section::OnPacket(PID: %d): Section discarded: bad CRC", this->pid->pid).c_str());
          } else {
            OnSection();
          }

          this->length=0;
          this->firstPacketIndex=0;
        }
      }
    } catch(...) {
      _logger.LogException(std::format("Section::OnPacket(): unhandled exception pid: %d 0x%04x packet: %" PRIu64 ".", this->pid->pid, this->pid->pid, packet->index).c_str());
    }
  }

  // ajm: -----------------------------------------------------------------------------------------
  void Section::OnSection() {
    try {
      // ajm: _logger.LogDebug("Section::OnSection()");

      auto header=reinterpret_cast<SectionHeader1 *>(this->payload);
      if(!header->section_syntax_indicator) {
        _logger.LogWarning(std::format("Section::OnSection(PID: %d) section_syntax_indicator[table_id=0x%02x] is 0.", this->pid->pid, header->table_id).c_str());
        // ajm: => private section plugin 
        return;
      }

      this->pid->transportStream->OnSection(this);  // ajm: => listeners

      Table *table=0;
      std::map<std::uint8_t, std::shared_ptr<Table>>::iterator i=this->pid->tables.find(header->table_id);	
      if(i!=this->pid->tables.end()) {
        table=(*i).second.get();

        if(table->versionNumber!=0xff&&header->version_number!=table->versionNumber) {
          _logger.LogInformation(std::format("Section::OnSection(PID: %d) %s version_number %d -> %d.", this->pid->pid, table->name, table->versionNumber, header->version_number).c_str());
          assert(0);  // ajm: untested ...
          this->pid->transportStream->device->OnState(ajm::framework::Device::eSTATE::eSTATE_STOP);
        }
      }

      switch(header->table_id) {
        case 0x00:
          if(!table) {
            table=new PAT(this);
            this->pid->tables.insert(std::map<std::uint8_t, std::shared_ptr<Table>>::value_type(header->table_id, std::shared_ptr<Table>(table)));
          }
          break;
        case 0x01:
          if(!table) {
            table=new CAT(this);
            this->pid->tables.insert(std::map<std::uint8_t, std::shared_ptr<Table>>::value_type(header->table_id, std::shared_ptr<Table>(table)));
          }
          break;
        case 0x02: {
          // ajm: new PMT => PAT::Entry::Entry(); 
          std::map<std::uint8_t, std::shared_ptr<Table>>::iterator j=this->pid->transportStream->pids[0]->tables.find(0x00);
          std::uint16_t serviceId=_GET_LE16(&((SectionHeader1 *)this->payload)->table_id_extension);
          if(j!=this->pid->tables.end()) {
            auto pat=(PAT *)(*j).second.get();
            table=pat->GetPMT(serviceId);
          }
          break;
        }
        default: { }
      }

      if(table) {
        std::map<std::uint32_t, std::shared_ptr<std::uint8_t>>::iterator j=table->sections.find(this->crc);
        if (j==table->sections.end()) { // ajm: new section
          auto payload=new std::uint8_t[this->length];
          std::memcpy(payload, this->payload, this->length);
          table->sections.insert(std::pair<std::uint32_t, std::shared_ptr<std::uint8_t>>(this->crc, std::shared_ptr<std::uint8_t>(payload)));
          table->OnSection();
        }

        if (header->last_section_number==table->sections.size()-1 && table->versionNumber==0xff) {
          _logger.LogInformation(std::format("Recieved table %s 0x%02x.", table->name.c_str(), table->tableId).c_str());
          table->Debug();
          table->versionNumber=header->version_number;  // ajm: table received
          this->pid->transportStream->OnTable(table);
        }
      }
    } catch(...) {
      _logger.LogException(std::format("Section::OnSection(): unhandled exception pid: %d 0x%04x.", this->pid->pid, this->pid->pid).c_str());
    }
  }

  void Section::Debug() {
    SectionHeader1 *header=reinterpret_cast<SectionHeader1 *>(this->payload); 
    std::printf("\nSECTION -------------------------------------------------------------------------");
    std::printf("\ntable id=0x%02x", header->table_id);	
    std::printf("\nsection syntax indicator=0x%02x", header->section_syntax_indicator);
    std::printf("\nprivate_indicator=0x%02x", header->private_indicator);	
    std::printf("\nreserved=0x%02x", header->reserved1);	
    std::uint16_t length=((std::uint16_t)header->length1<<8)|header->length2;
    std::printf("\nlength=0x%02x", length);	
    std::printf("\ntable_id_extension=0x%04x", header->table_id_extension);
    std::printf("\nreserved=0x%02x", header->reserved2);	
    std::printf("\nversion number=0x%02x", header->version_number);	
    std::printf("\ncurrent next indicator=0x%02x", header->currentNext_indicator);	
    std::printf("\nsection number=0x%02x", header->section_number);	
    std::printf("\nlast section number=0x%02x", header->last_section_number);	
    std::printf("\nsection[8]=data, %d bytes", length-4);
  }
}
