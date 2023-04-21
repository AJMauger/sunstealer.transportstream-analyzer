#include <algorithm>
#include <deque>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../sunstealer.cpp/cxx.h"

#include "../sunstealer.framework/logger.h"

#include "../sunstealer.system/format.hpp"
#include "../sunstealer.system/object.hpp"
#include "../sunstealer.system/system.hpp"

#include "../sunstealer.framework/device.hpp"
#include "../sunstealer.framework/document.hpp"
#include "../sunstealer.framework/logger.h"
#include "../sunstealer.framework/plugin.h"

#include "../sunstealer.iso-13818-1/crc32.hpp"
#include "../sunstealer.iso-13818-1/descriptor.h"
#include "../sunstealer.iso-13818-1/packet.h"
#include "../sunstealer.iso-13818-1/pid.h"
#include "../sunstealer.iso-13818-1/section.h"
#include "../sunstealer.iso-13818-1/table.h"
#include "../sunstealer.iso-13818-1/transportstream.h"

#include "./main.h"
#include "./plugin.hpp"

namespace ajm::iso13818_1_serviceinformation {
  Plugin *Plugin::_plugin=nullptr;
}

namespace ajm::system {
  System *System::_system=nullptr;
}

// ajm: ---------------------------------------------------------------------------------------
extern "C" {
  ajm::framework::Device *Create(ajm::framework::ILogger *logger, std::map<std::string, std::string> parameters) {
    ajm::system::System::GetInstance(logger);
    ajm::iso13818_1_serviceinformation::Plugin::GetInstance(parameters);
    _logger.LogDebug(std::format("ajm::iso13818_1_service-information::Create(): _plugin 0x%" PRIx64 ".", ajm::iso13818_1_serviceinformation::Plugin::_plugin).c_str());
    return (dynamic_cast<ajm::framework::Device *>(ajm::iso13818_1_serviceinformation::Plugin::_plugin));
  }

  void Destroy() {
    if (ajm::iso13818_1_serviceinformation::Plugin::_plugin) {
      delete ajm::iso13818_1_serviceinformation::Plugin::_plugin;
      ajm::iso13818_1_serviceinformation::Plugin::_plugin = 0;
    }
  }

  std::string GetName() {
    return "iso-13818-1::service-information";
  };
}

