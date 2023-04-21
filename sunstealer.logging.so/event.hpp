#pragma once

namespace ajm::logging {
  enum eSEVERITY {
    eDEBUG = 0,
    eINFORMATION = 1,
    eWARNING = 2,
    eERROR = 3,
    eEXCEPTION = 4
  };

  const char *severity[5] = {"Db", "In", "Wn", "Er", "Ex"};

  class Event {
    public: Event(eSEVERITY severity, const std::string message, std::string *notification) {
      this->severity = severity;
      this->message = message;
      // this->notification_ = notification;
      struct timeval tv;
      gettimeofday(&tv, nullptr);
      this->time = *gmtime(&tv.tv_sec);
      std::sprintf(&this->date[0] , "20%02d-%02d-%02dT%02d:%02d:%02d.%03d", this->time.tm_year - 100, this->time.tm_mon, this->time.tm_mday, this->time.tm_hour, this->time.tm_min, this->time.tm_sec, tv.tv_usec/1000);
    }

    public: std::string date=std::string(0x20, 24);
    public: std::string id;
    public: std::string message;
    public: std::string notification;
    public: eSEVERITY severity;
    public: tm time;
  };
}
