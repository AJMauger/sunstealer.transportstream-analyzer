#pragma once

#include <exception>
#include <deque>
#include <filesystem>
#include <fstream>

#include "../sunstealer.system/format.hpp"
#include "../sunstealer.system/object.hpp"

#include "../sunstealer.framework/logger.h"

#include "./event.hpp"

using namespace std::string_literals;

namespace ajm::logging {
  class Logger : public ajm::system::Object, public ajm::framework::ILogger {
    public:Logger(int fileSize, std::string folder, int memorySize, std::string name)
      : Object(name) {
      this->fileSize = fileSize;
      this->folder = folder;
      this->memorySize = memorySize;

      if (!std::filesystem::exists(this->folder)) {
        std::filesystem::create_directories(this->folder);
      }
      this->Open();
    }

    // ajm: ---------------------------------------------------------------------------------------
    public: ~Logger() {
      this->Close();
      std::printf("Logger::~Logger() deleting %d events.", this->events.size());
      for (std::deque<Event *>::iterator i = this->events.begin(); i != this->events.end(); i++) {
        delete *i;
      }
      this->events.clear();
    }

    // ajm: ---------------------------------------------------------------------------------------
    public: void Close() {
      try {
        Event event(ajm::logging::eINFORMATION, "", nullptr);
        std::string s = std::format("%s [In] Log closed\r\n", event.date.c_str());
        this->file.write(s.c_str(), s.size());
        this->file.close();
        std::printf(s.c_str());
        this->fileCount = 0;
      } catch(...) {
        std::printf("Logger::Close() unhanded exception");
      }
    }

    // ajm: ---------------------------------------------------------------------------------------
    public: void LogDebug(const char *message) {
      this->Log(eSEVERITY::eDEBUG, message);
    }

    public: void LogInformation(const char *message) {
      this->Log(eSEVERITY::eINFORMATION, message);
    }

    public: void LogWarning(const char *message) {
      this->Log(eSEVERITY::eWARNING, message);
    }

    public: void LogError(const char *message) {
      this->Log(eSEVERITY::eERROR, message);
    }

    public: void LogException(const char *message) {
      this->Log(eSEVERITY::eEXCEPTION, message);
    }

    // ajm: ---------------------------------------------------------------------------------------
    public: void Log(eSEVERITY severity, const std::string message) {
      try {
        this->lock.lock();
        if (events.size() == this->memorySize) {
          std::deque<Event *>::iterator i = events.begin();
          events.pop_front();
          delete *i;
        }

        this->fileCount++;
        if (this->fileCount == this->fileSize)
        {
          this->Close();
          this->Open();
        }

        Event *event = new Event(severity, message, nullptr);
        events.push_back(event);
        std::string s = std::format("%s [%s] %s\r\n", event->date.c_str(), ajm::logging::severity[severity], event->message.c_str());
        this->file.write(s.c_str(), s.size());
        this->file.flush();
        std::printf(s.c_str());
        this->lock.unlock();
      } catch(...) {
        std::printf("Logger::Log() unhanded exception");
      }
    }

    // ajm: ---------------------------------------------------------------------------------------
    public: void Open() {
      try {
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        struct tm t = *gmtime(&tv.tv_sec);
        std::string uri = std::format("%s/20%02d-%02d-%02dT%02d:%02d:%02d.log", this->folder.c_str(), t.tm_year - 100, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);
        this->file.open(uri.c_str(), std::fstream::out);

        Event event(ajm::logging::eINFORMATION, "", nullptr);
        std::string s = std::format("%s [In] Log opened.\r\n", event.date.c_str());
        this->file.write(s.c_str(), s.size());
        std::printf(s.c_str());
      } catch(...) {
        std::printf("Logger::Open() unhanded exception");
      }
    }

    // ajm: ---------------------------------------------------------------------------------------
    private: std::deque<Event *> events;
    private: std::fstream file;
    private: std::string folder;
    private: std::int32_t fileCount = 0;
    private: std::int32_t fileSize = 0;
    private: std::mutex lock;
    private: std::int32_t memorySize = 0;
  };
}
