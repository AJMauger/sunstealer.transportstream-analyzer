#include "../sunstealer.framework/logger.h"

#include <sys/time.h>
#include <memory>
#include <string>

#include "../sunstealer.system/format.hpp"

#include "./logger.hpp"
#include "./main.h"

ajm::logging::Logger *_logger = nullptr;

// ajm: ---------------------------------------------------------------------------------------
extern "C" {
  ajm::framework::ILogger *Create(int fileSize, const char *folder, int memorySize, const char *name) {
    _logger = new ajm::logging::Logger(fileSize, folder, memorySize, name);
    return dynamic_cast<ajm::framework::ILogger *>(_logger);
  }

  void Destroy(const char *name) {
    delete _logger;
  }
}
