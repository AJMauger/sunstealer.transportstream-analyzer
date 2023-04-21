#include <memory>
#include <string>
#include <vector>

#include "../sunstealer.system/object.hpp"

#include "./crc32.hpp"
#include "./descriptor.h"
#include "./packet.h"
#include "./pid.h"
#include "./section.h"
#include "./table.h"
#include "./cat.h"

namespace ajm::iso13818_1 {
  CAT::CAT(Section *section) 
    : Table("CAT", section, 0x01) {};
 
  void CAT::OnSection() {
    std::uint8_t *stream=this->section->payload;
    *stream+=sizeof(SectionHeader1);
  	std::uint8_t *end=stream+this->section->length-9;

	while(stream<end)
		this->descriptors.push_back(std::shared_ptr<Descriptor>(new Descriptor(&stream, this)));
  }
}