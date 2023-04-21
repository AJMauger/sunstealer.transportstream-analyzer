#pragma once 

namespace ajm::iso13818_1 {
  // ajm: -----------------------------------------------------------------------------------------
  class TransportStream : public ajm::framework::Document {
    public: explicit TransportStream(std::string name);
    public: virtual ~TransportStream();

    public: virtual void OnAdaptationField(Packet *packet, PID *pid);
    public: virtual void OnPacket(Packet *packet);
    public: virtual void OnPID(PID *pid);
    public: virtual void OnSection(Section *section);
    public: virtual void OnTable(Table *table);

    PID *pids[0x2000];
  };
}

