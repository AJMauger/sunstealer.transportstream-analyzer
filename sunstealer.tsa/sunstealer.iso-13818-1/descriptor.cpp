#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../sunstealer.system/format.hpp"
#include "../sunstealer.system/object.hpp"

#include "./crc32.hpp"
#include "./descriptor.h"
#include "./packet.h"
#include "./pid.h"
#include "./section.h"
#include "./table.h"

namespace ajm::iso13818_1 {
  Descriptor::Descriptor(std::uint8_t **stream, Table *table)
    : ajm::system::Object(GetName(*stream[0], table->name)), stream(*stream), table(table) {

    (*stream)+=this->stream[1]+2;
  }

  std::string Descriptor::GetName(std::uint8_t tag, std::string &table){
    switch(tag) {
      // AJM: iso-13818-1
      case 0x00:
        return table + " 0x00 reserved";
      break;
      case 0x01:
        return table + " 0x01 reserved";
      break;
      case 0x02:
        return table + " 0x02 video_stream";
      break;
      case 0x03:
        return table + " 0x03 audio_stream";
      break;
      case 0x04: 
        return table + " 0x04 hierarchy";
      break;
      case 0x05:
        return table + " 0x05 registration";
      break;
      case 0x06:
        return table + " 0x06 data_stream_alignment";
      break;
      case 0x07:
        return table + " 0x07 target_background_grid";
      break;
      case 0x08:
        return table + " 0x08 video_window";
      break;
      case 0x09:
        return table + " 0x09 CA";
      break;
      case 0x0a:
        return table + " 0x0a ISO_639_language";
      break;
      case 0x0b:
        return table + " 0x0b system_clock";
      break;
      case 0x0c: 
        return table + " 0x0c multiplex_buffer_utilization";
      break;
      case 0x0d: 
        return table + " 0x0d copyright";
      break;
      case 0x0e: 
        return table + " 0x0e maximum_bitrate";
      break;
      case 0x0f: 
        return table + " 0x0f private_data_indicator";
      break;
      case 0x10: 
        return table + " 0x10 smoothing_buffer";
      break;
      case 0x11: 
        return table + " 0x11 STD";
      break;
      case 0x12: 
        return table + " 0x12 IBP";
      break;

      // AJM: iso-13818-6
      case 0x13:
        return table + " 0x13 carousel_id";
      break;
      case 0x14:
        return table + " 0x14 association_tag";
      break;
      case 0x15:
        return table + " 0x15 deferred_association_tag";
      break;

      case 0x1b:
        return table + " 0x1b h.264_video";
      break;
      case 0x1c:
        return table + " 0x1c MPEG-4_audio";
      break;
      case 0x1d:
        return table + " 0x1d IOD";
      break;
      case 0x1e:
        return table + " 0x1e SL";
      break;
      case 0x1f:
        return table + " 0x1f FMC";
      break;
      case 0x20:
        return table + " 0x20 external_ES_ID";
      break;
      case 0x21:
        return table + " 0x21 MuxCode";
      break;
      case 0x22:
        return table + " 0x22 FmxBufferSize";
      break;
      case 0x23:
        return table + " 0x23 multiplexbuffer";
      break;
      case 0x24:
        return table + " 0x24 h.265_video";
      break;
      case 0x25:
        return table + " 0x25 metadata_pointer";
      break;
      case 0x26:
        return table + " 0x26 metadata";
      break;
      case 0x27:
        return table + " 0x27 metadata_STD";
      break;
      case 0x28:
        return table + " 0x28 AVC_video";
      break;
      case 0x29:
        return table + " 0x29 IPMP (defined in iso/iec 13818-11, mpeg-2 ipmp)";
      break;
      case 0x30:
        return table + " 0x30 AVC_timing_and_HRD";
      break;
      case 0x31:
        return table + " 0x31 MPEG-2_AAC_audio";
      break;
      case 0x32:
        return table + " 0x32 FlexMuxTiming";
      break;

      // AJM: hevc
      case 0x38:
        return table + " 0x38 hevc_video_descriptor";
      break;
        
      // AJM: dvb
      case 0x45:
        return table + " 0x45 vbi_data_descriptor";
      break;
      case 0x46:
        return table + " 0x46 vbi_teletext_descriptor";
      break;
      case 0x52:
        return table + std::format(" 0x52 stream_id, component_tag: 0x%02x", this->stream[2]);
      break;
      case 0x66:
        return table + " 0x66 data_broadcast_id";
      break;
      case 0x6f:
        return table + " 0x6f ait";
      break;
      case 0x70:
        return table + " 0x70 adaptation_field_data_descriptor";
      break;
      case 0x7a:
        return table + " 0x7a enhanced ac-3 descriptor";
      break;

      case 0x80:
        return table + " 0x66 application";
      break;
      case 0x97:
        return table + " 0x97 scte_adaptation_field_data_descriptor";
      break;
      case 0xcc:
        return table + " 0xcc e-ac-3_audio_descriptor";
      break;
      case 0xe9:
        return table + " 0xe9 encoder_boundary_point_descriptor"; 
      break;
      default:
        return table + std::format(" 0x%02x unknown", tag);
    }
  }
}