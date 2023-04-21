#pragma once

namespace ajm::iso13818_1 {
  class CRC32 {
    public: CRC32() {
      std::uint32_t crc=0, i=0, j=0;

      for(i=0; i<256; i++) {
        crc=(i<<24);
        for(j=0; j<8; j++) {
          if(crc&0x80000000)
              crc=(crc<<1)^0x04c11db7;
          else
              crc=(crc<<1);
        }
        table[i]=crc;
      }
    }
    
    public: virtual ~CRC32() {}

    public: std::uint32_t Calc(std::uint8_t *data, std::uint32_t length) {
      std::uint8_t *ptr=data;
      std::uint32_t crc=0xffffffff;

      for(std::uint32_t i=0; i<length; i++, ptr++)
        crc=(crc<<8)^table[(crc>>24)^*ptr];
      return crc;
    }

    private: std::uint32_t table[256];
  };
}