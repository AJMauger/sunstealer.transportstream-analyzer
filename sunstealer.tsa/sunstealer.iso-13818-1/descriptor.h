#pragma once

namespace ajm::iso13818_1 {
  class Table;

  // ajm: -----------------------------------------------------------------------------------------
  class Descriptor : public ajm::system::Object {
    public: Descriptor(std::uint8_t **stream, Table *table);
    public: virtual ~Descriptor() = default;
    public: std::string GetName(std::uint8_t tag, std::string &table);

    public: std::uint8_t *stream;
    public: Table *table;
  };
}
