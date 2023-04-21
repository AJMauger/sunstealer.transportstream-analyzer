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
  PAT::PAT(Section *section) 
    : Table("PAT", section, 0x00) {

      _logger.LogDebug("PAT::PAT()");
    }
 
  // ajm: -----------------------------------------------------------------------------------------
  void PAT::Debug() {
    std::printf("\nTABLE PAT ------------------------------------------------------------------------");
    for (auto i=this->entries.begin(); i!=this->entries.end(); i++) {
      const PAT::Entry *entry=dynamic_cast<PAT::Entry *>((*i).get());
      std::printf("\n{");
      std::printf("\n  pid: %d 0x%04x", entry->pid->pid, entry->pid->pid);
      std::printf("\n  service_id: %d", entry->serviceId);
      std::printf("\n}\n\n");
    }
  }

  // ajm: -----------------------------------------------------------------------------------------
  PMT* PAT::GetPMT(std::uint16_t serviceId) {
    for (auto i=this->entries.begin(); i!=this->entries.end(); i++) {
      PMT *pmt=((PAT::Entry *)(*i).get())->pmt;
      if(pmt->serviceId==serviceId)
        return pmt;
    }
    return nullptr;
  }

  // ajm: -----------------------------------------------------------------------------------------
  void PAT::OnSection() {
    std::uint8_t *stream=this->section->payload;
    stream+=sizeof(SectionHeader1);
    const std::uint8_t *end=stream+this->section->length-9;
    while(stream<end) {
      this->entries.push_back(std::make_shared<PAT::Entry>(this->name, &stream, this));
    }
    _logger.LogDebug(std::format("PAT::OnSection() entries: %d.", this->entries.size()).c_str());
  }

  // ajm: -----------------------------------------------------------------------------------------
  PAT::Entry::Entry(std::string &name, std::uint8_t **stream, PAT *table)
    : Table::Entry(name, table)  {

      this->serviceId=_GET_LE16(*stream);
      (*stream)+=2;

      std::uint16_t pid=_GET_LEPID(*stream);
      (*stream)+=2;

      if(!table->section->pid->transportStream->pids[pid]) {
        table->section->pid->transportStream->pids[pid]=new PID(pid, table->section->pid->transportStream);
        table->section->pid->transportStream->OnPID(table->section->pid->transportStream->pids[pid]);
      }
      table->section->pid->transportStream->pids[pid]->type=PID::eTYPE::eTYPE_SECTION;
      this->pid=table->section->pid->transportStream->pids[pid];

      if(!this->serviceId) {
        _logger.LogInformation(std::format("PAT::Entry::Entry() network_pid=0x%04x", pid).c_str());
      }

      if(!table->section->pid->transportStream->pids[pid]->section) {
        table->section->pid->transportStream->pids[pid]->section=new Section(table->section->pid->transportStream->pids[pid]);
      }
      auto *header=reinterpret_cast<SectionHeader1 *>(table->section->pid->transportStream->pids[pid]->section->payload); // ajm: cast?
      header->section_syntax_indicator=1;
      header->last_section_number=0;
      
      this->pmt=new PMT(table, table->section->pid->transportStream->pids[pid]->section, table->entries.size(), this->serviceId);
      table->section->pid->transportStream->pids[pid]->tables.insert(std::map<std::uint8_t, Table *>::value_type(0x02, this->pmt));
    };
}