#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../sunstealer.framework/logger.h"

#include "../sunstealer.system/defines.h"
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
#include "./pat.h"
#include "./pmt.h"

namespace ajm::iso13818_1 {
  PMT::PMT(PAT *pat, Section *section, std::int8_t index, std::uint16_t serviceId)
    : Table("PMT", section, 0x02), index(index), pcr(0), programInfoLength(0), serviceId(serviceId) {

      _logger.LogDebug("PMT::PMT()");
    }

  // ajm: -----------------------------------------------------------------------------------------
  void PMT::Debug() {
    std::printf("\nTABLE PMT ------------------------------------------------------------------------");
    std::printf("\n{");
    std::printf("\n  service_id: %d 0x%04x", this->serviceId, this->serviceId);
    std::printf("\n  pcr_pid: %d 0x%04x", this->pcr->pid, this->pcr->pid);
    std::printf("\n  program_info_descriptors[%d]: {", this->descriptors.size());
    for(auto i=this->descriptors.begin(); i!=this->descriptors.end(); i++) {
      std::printf("\n  tag: %d 0x%02x %s", (*i)->stream[0], (*i)->stream[0], (*i)->name.c_str());
    }
    std::printf("\n  }");
    for(auto i=this->entries.begin(); i!=this->entries.end(); i++) {
      std::printf("\n  entry {");
      auto *entry=dynamic_cast<PMT::Entry *>((*i).get());
      std::printf("\n    es_info_descriptors[%d]: {", entry->descriptors.size());
      for(auto j=entry->descriptors.begin(); j!=entry->descriptors.end(); j++) {
        std::printf("\n      tag: %d %s", (*j)->stream[0], (*j)->name.c_str());
      }
      std::printf("\n    }");
      std::printf("\n    stream_type: 0x%02x", entry->streamType);
      std::printf("\n    pcr_pid: %d 0x%04x", entry->pid->pid, entry->pid->pid);
      std::printf("\n  }");
    }
    std::printf("\n}\n\n");
  }

  // ajm: -----------------------------------------------------------------------------------------
  void PMT::OnSection() {
    _logger.LogDebug("PMT::OnSection().");

    std::uint8_t *stream=this->section->payload;
    stream+=sizeof(SectionHeader1);
    
    std::uint16_t pid=_GET_LEPID(stream);
    stream+=2;
    if(!this->section->pid->transportStream->pids[pid]) {
      this->section->pid->transportStream->pids[pid]=new PID(pid, this->section->pid->transportStream);
      this->section->pid->transportStream->pids[pid]->type=PID::eTYPE::eTYPE_SECTION;
      this->section->pid->transportStream->OnPID(this->section->pid->transportStream->pids[pid]);
    }
    this->pcr=this->section->pid->transportStream->pids[pid];

    this->programInfoLength=((*stream&0x0f)<<8)|stream[1];
    stream+=2;

    const std::uint8_t *end=stream+this->programInfoLength;
    while(stream<end) {
      this->descriptors.push_back(std::make_shared<Descriptor>(&stream, this));
    }

    end=stream+this->section->length-this->programInfoLength-13;	// ajm: -(9+4)
    while(stream<end) {
      this->entries.push_back(std::make_shared<PMT::Entry>(this->name, &stream, this));
    }

    _logger.LogDebug(std::format("PMT::OnSection() descriptors: %d, entries: %d.", this->descriptors.size(), this->entries.size()).c_str());
  }

  // ajm: -----------------------------------------------------------------------------------------
  PMT::Entry::Entry(std::string &name, std::uint8_t **stream, Table *table)
    : Table::Entry(name, table) {

    try {
      this->streamType=**stream;
      (*stream)++;

      std::uint16_t pid=_GET_LEPID(*stream);
      (*stream)+=2;

      // AJM: pid
      if(!table->section->pid->transportStream->pids[pid]){	
        table->section->pid->transportStream->pids[pid]=new PID(pid, table->section->pid->transportStream);
        table->section->pid->transportStream->OnPID(table->section->pid->transportStream->pids[pid]);
      }

      switch(this->streamType) {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x06:
        case 0x1b:	// AJM: h.264
        case 0x24:	// AJM: h.265
        case 0x80:	
        case 0x81: {	
          // AJM: pes
          table->section->pid->transportStream->pids[pid]->type=PID::eTYPE::eTYPE_PACKETIZED_ELEMENTARY_STREAM;
          break;
        }
        case 0x05:
        case 0x07:
        case 0x08:
        case 0x09:
        case 0x0a:
        case 0x0b:
        case 0x0c:
        case 0x0d:
        case 0x82:
        case 0x86:
        case 0xc0: {
          // AJM: section
          table->section->pid->transportStream->pids[pid]->type=PID::eTYPE::eTYPE_SECTION;
          break;
        }
      }
      this->pid=table->section->pid->transportStream->pids[pid];

      this->esInfoLength=((**stream&0x0f)<<8)+(*stream)[1];
      (*stream)+=2;;

      std::uint8_t *end=(*stream)+this->esInfoLength;
      while(*stream<end) {
        this->descriptors.push_back(std::make_shared<Descriptor>(stream, table));
      }
    } catch(...) {
      _logger.LogException("PMT::Entry::Entry(): unhandled exception.");
    }
  }
}
