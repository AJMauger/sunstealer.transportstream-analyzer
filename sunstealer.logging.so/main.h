#pragma once

#include "../sunstealer.framework/logger.h"
#include "./logger.hpp"

// ajm: ---------------------------------------------------------------------------------------
extern "C" {
  ajm::framework::ILogger *Create(int fileSize, const char *folder, int memorySize, const char *name);
  void Destroy(const char *name);
}
