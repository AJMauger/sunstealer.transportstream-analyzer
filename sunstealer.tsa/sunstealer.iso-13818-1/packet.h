#pragma once

namespace ajm::iso13818_1 {
  // ajm: -----------------------------------------------------------------------------------------
  class Packet {
    public: Packet(const std::uint8_t *packet, std::int64_t index);

    public: std::uint8_t payload[188];
    public: std::int64_t index;
  };

  // ajm: -----------------------------------------------------------------------------------------
  class PacketHeader {
    public: std::uint8_t sync;
    
    public: std::uint8_t pid1 : 5; // ajm: 5 bit
    public: std::uint8_t transport_priority : 1; // ajm: 1 bit
    public: std::uint8_t payload_start_indicator : 1; // ajm: 1 bit
    public: std::uint8_t transport_error_indicator : 1; // ajm: 1 bit
    
    public: std::uint8_t pid2;
    
    public: std::uint8_t continuity_counter : 4; // ajm: 4 bit
    public: std::uint8_t adaptation_field_control : 2; // ajm: 2 bit
    public: std::uint8_t transport_scrambling_control : 2; // ajm: 2 bit

    public: std::uint8_t adaptation_field_length;
    
    public: std::uint8_t adaptation_field_extensionFlag : 1; // ajm: 1 bit
    public: std::uint8_t transport_private_data_flag : 1; // ajm: 1 bit
    public: std::uint8_t splicing_point_flag : 1; // ajm: 1 bit
    public: std::uint8_t opcr_flag : 1; // ajm: 1 bit
    public: std::uint8_t pcr_flag : 1; // ajm: 1 bit
    public: std::uint8_t elementary_stream_priority_indicator : 1; // ajm: 1 bit
    public: std::uint8_t random_access_indicator : 1; // ajm: 1 bit
    public: std::uint8_t discontinuity_indicator : 1; // ajm: 1 bit
  };
}