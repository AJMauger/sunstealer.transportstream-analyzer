#include <algorithm>
#include <cstring>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../sunstealer.system/object.hpp"

#include "../sunstealer.framework/document.hpp"

#include "./crc32.hpp"
#include "./descriptor.h"
#include "./packet.h"
#include "./pid.h"
#include "./section.h"
#include "./table.h"
#include "./transportstream.h"

namespace ajm::iso13818_1 {
  TransportStream::TransportStream(std::string name) 
    : ajm::framework::Document(name) {

      std::memset(this->pids, 0, sizeof(this->pids));
    }

  TransportStream::~TransportStream() {
    for (std::uint16_t i=0; i<sizeof(pids)/sizeof(PID *); i++) {
      if(pids[i] && pids[i]->transportStream==this) {
        delete pids[i];
      }
    }
  }

  void TransportStream::OnAdaptationField(Packet *packet, PID *pid) {
    for (auto i=this->documents.begin(); i!=this->documents.end(); i++) {
      auto ts=dynamic_cast<TransportStream *>(*i);
      if (ts) {
        ts->OnAdaptationField(packet, pid);
      }
    }
  }

  void TransportStream::OnPacket(Packet *packet) {
    for (auto i=this->documents.begin(); i!=this->documents.end(); i++) {
      auto ts=dynamic_cast<TransportStream *>(*i);
      if (ts) {
        ts->OnPacket(packet);
      }
    }
  }

  void TransportStream::OnPID(PID *pid) {
    for (auto i=this->documents.begin(); i!=this->documents.end(); i++) {
      auto ts=(dynamic_cast<TransportStream *>(*i));
      if (ts) {
        ts->OnPID(pid);
      }
    }
  }

  void TransportStream::OnSection(Section *section) {
    for (auto i=this->documents.begin(); i!=this->documents.end(); i++) {
      auto ts=dynamic_cast<TransportStream *>(*i);
      if (ts) {
        ts->OnSection(section);
      }
    }
  }

  void TransportStream::OnTable(Table *table) {
    for (auto i=this->documents.begin(); i!=this->documents.end(); i++) {
      auto ts=(dynamic_cast<TransportStream *>(*i));
      if (ts) {
        ts->OnTable(table);
      }
    }
  }
}

