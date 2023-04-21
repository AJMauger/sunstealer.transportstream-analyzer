#include <dlfcn.h>
#include <signal.h>

#include "cxx.h"

// ajm: #include <boost/asio.hpp>

#include "../sunstealer.framework/logger.h"

#include "../sunstealer.system/object.hpp"
#include "../sunstealer.system/format.hpp"
#include "../sunstealer.system/system.hpp"
#include "../sunstealer.system/thread.hpp"

#include "../sunstealer.framework/device.hpp"
#include "../sunstealer.framework/document.hpp"

// ajm: integrated sunstealer.logging
// ajm: #include "../sunstealer.logging.so/event.hpp"
// ajm: #include "../sunstealer.logging.so/logger.hpp"

#include "../sunstealer.iso-13818-1/crc32.hpp"
#include "../sunstealer.iso-13818-1/descriptor.h"
#include "../sunstealer.iso-13818-1/packet.h"
#include "../sunstealer.iso-13818-1/pid.h"
#include "../sunstealer.iso-13818-1/section.h"
#include "../sunstealer.iso-13818-1/table.h"
#include "../sunstealer.iso-13818-1/transportstream.h"
#include "../sunstealer.iso-13818-1-file.so/main.h"

#include "./transportstreamanalyzer.hpp"

namespace ajm::system {
  System *System::_system=nullptr;
}

// ajm: -------------------------------------------------------------------------------------------
void sasigaction (int signum, siginfo_t *siginfo, void *ucontext) {
  std::printf("\n\nsasigaction: sig=%d, uap=%p\n\n", siginfo->si_signo, ucontext);

/* ajm: #ifdef __x86_64__
  ((ucontext_t *)ucontext)->uc_mcontext ;
#endif*/

  exit(0);
}

// ajm: -------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  try {
    if(argc % 2 != 1) {
      std::printf("Usage: ./sunstealer -c transportstreamanalyzer");
      return -1;
    }

    std::map<std::string, std::string> options;
    for (int i=1; i<argc; i+=2) {
      options.try_emplace(argv[i], argv[i+1]);
    }

    for (std::map<std::string, std::string>::iterator i=options.begin(); i!=options.end(); i++) {
      std::printf("DEBUG; options: %s %s.\n", (*i).first.c_str(), (*i).second.c_str());
    }

    struct sigaction sa;
    std::memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sasigaction;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, 0);

    // ajm: itegrated | static sunstealer.logging.so
    // ajm::logging::Logger *logger = new ajm::logging::Logger(10000, "/home/ajm/Documents/sunstealer/Logs", 500, "logger");

    std::uint32_t endian=1;
    std::printf("DEBUG: %s endian.\n", *(std::uint8_t *)&endian ? "little" : "big");

    std::printf("DEBUG: std::_S_word_bit %d bits.\n", std::_S_word_bit);
    std::printf("DEBUG: int %d bits.  std::int_32_t %d bits.  std::int_fast32_t %d bits.\n", 8*sizeof(int), 8*sizeof(std::int32_t), 8*sizeof(std::int_fast32_t));
    std::printf("DEBUG: void* %d bits.\n", 8*sizeof(void *));
    
    std::printf("DEBUG: main std::this_thread::id: 0x%" PRIx64 ".\n", std::this_thread::get_id());
    std::printf("DEBUG: std::filesystem::current_path '%s'.\n\n", std::filesystem::current_path().c_str());

    // ajm: shared sunstealer.logging.so
    void *handle = dlopen("../sunstealer.logging.so/build/libsunstealer.logging.so", RTLD_LOCAL | RTLD_LAZY);
    if (handle) {
      auto (*create)(int fileSize, const char *folder, int memorySize, const char *name)
        = (ajm::framework::ILogger * (*)(int fileSize, const char *folder, int memorySize, const char *name)) dlsym(handle, "Create");

      auto (*destroy)(const char *name) = (ajm::framework::ILogger * (*)(const char *name)) dlsym(handle, "Destroy");

      if (create && destroy) {
        const char *loggerName = "logger";
        ajm::framework::ILogger *logger = create(10000, "/home/ajm/Documents/sunstealer/Logs", 500, loggerName);
        if (logger) {
          ajm::system::System::GetInstance(logger);
        } else {
          std::printf("ERROR: logger == 0.\n");
        }

        if (ajm::system::System::_system) {        
          std::string command = (*options.find("-c")).second; 
          if (command == "transportstreamanalyzer") {
            ajm::transportstreamanalyzer::TransportStreamAnalyzer tsa(logger);
            tsa.LoadPlugins();
            tsa.Analyze("/home/ajm/Videos/Space.ts");
          }

          destroy(loggerName);
        } else {
          std::printf("ERROR: _system == 0.\n");
        }

        dlclose(handle);
      } else {
        std::printf("ERROR: pLoggerCreate == 0 || pLoggerDestroy == 0.\n");
      }
    } 
    else {
      std::printf("ERROR: handle == 0.\n");
    }

    options.clear();

    delete ajm::system::System::_system;

    std::printf("\n\n");
  } catch(...) {
    std::printf("EXCEPTION: Unhandled exception.");
  }
}
