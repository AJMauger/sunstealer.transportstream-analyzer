#pragma once

#define SECTION_LENGTH 4*1024+188

namespace ajm::iso13818_1 {
  // ajm: -----------------------------------------------------------------------------------------
  class Section {
    public: explicit Section(PID *pid);
    public: virtual void OnPacket(Packet *packet);
  	public: virtual void OnSection();

    public: void Debug();

    private: CRC32 crc32;

    public: std::uint8_t continuityCounter;
    public: std::uint32_t crc;
    public: std::uint16_t length;
    public: std::uint16_t payloadLength;
    public: PID *pid=0;
    public: std::uint8_t payload[SECTION_LENGTH];
    public: std::uint64_t firstPacketIndex;
    public: std::uint64_t lastPacketIndex;
  };

  // ajm: -----------------------------------------------------------------------------------------
  class SectionHeader0 {
    public: std::uint8_t table_id;	

    public: std::uint8_t length1 : 4; // ajm: 4 bit
    public: std::uint8_t reserved1 : 2; // ajm: 2 bit
    public: std::uint8_t private_indicator : 1; // ajm: 1 bit
    public: std::uint8_t section_syntax_indicator : 1; // ajm: 1 bit

    public: std::uint8_t length2;
  };

  // ajm: -----------------------------------------------------------------------------------------
  class SectionHeader1  {
    public: std::uint8_t table_id;	

    public: std::uint8_t length1 : 4; // ajm: 4 bit
    public: std::uint8_t reserved1 : 2; // ajm: 2 bit
    public: std::uint8_t private_indicator : 1; // ajm: 1 bit
    public: std::uint8_t section_syntax_indicator : 1; // ajm: 1 bit

    public: std::uint8_t length2;

    public: std::uint8_t table_id_extension[2];
 
    public: std::uint8_t currentNext_indicator : 1; // ajm: 1 bit
    public: std::uint8_t version_number : 5; // ajm: 5 bit
    public: std::uint8_t reserved2 : 2; // ajm: 2 bit
 
    public: std::uint8_t section_number;
 
    public: std::uint8_t last_section_number;
  };
}