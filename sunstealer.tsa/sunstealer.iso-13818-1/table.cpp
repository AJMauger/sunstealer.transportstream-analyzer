#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../sunstealer.framework/logger.h"

#include "../sunstealer.system/format.hpp"
#include "../sunstealer.system/object.hpp"
#include "../sunstealer.system/system.hpp"

#include "./crc32.hpp"
#include "./descriptor.h"
#include "./packet.h"
#include "./pid.h"
#include "./section.h"
#include "./table.h"

namespace ajm::iso13818_1 {
  Table::Table(const std::string &name, Section *section, std::uint8_t tableId)
    : ajm::system::Object(name), section(section), tableId(tableId), versionNumber(0xff) {

    _logger.LogInformation(std::format("Table::Table(PID: %d table_id: 0x%02x)", section->pid->pid, tableId).c_str());
  }

  Table::~Table() {
    this->descriptors.clear();
    this->entries.clear();
    this->sections.clear();
  }

  Table::Entry::Entry(const std::string &name, Table *table)
    : ajm::system::Object(name) {}
  
  Table::Entry::~Entry() {
    this->descriptors.clear();
  }
}
 
