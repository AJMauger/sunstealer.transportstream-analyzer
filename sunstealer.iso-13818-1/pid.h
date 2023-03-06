#pragma once

namespace ajm::iso13818_1 {
  class Section;
  class TransportStream;

  // ajm: -----------------------------------------------------------------------------------------
  class PID {
    public: enum eTYPE {
      eTYPE_UNKNOWN=0,
      eTYPE_PACKETIZED_ELEMENTARY_STREAM=1,
      eTYPE_SECTION=2
    };    
    public: eTYPE type;

    public: PID(std::uint16_t pid, TransportStream *transportStream);
    public: virtual ~PID();

    public: virtual void OnPacket(Packet *packet);

  	public: std::map<std::uint8_t, std::shared_ptr<Table>> tables;
    public: std::uint32_t count;
    public: std::uint16_t pid;
    public: Section *section;
    public: TransportStream *transportStream=0;
  };
}
