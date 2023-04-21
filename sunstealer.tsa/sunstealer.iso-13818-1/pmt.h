#pragma once

namespace ajm::iso13818_1 {
  class PAT;

  // ajm: -----------------------------------------------------------------------------------------
  class PMT : public Table {
    public: PMT(PAT *pat, Section *section, std::int8_t index, std::uint16_t serviceId);
    public: virtual ~PMT() = default;

    public: void Debug();
    public: virtual void OnSection();

    public: std::int8_t index;
    public: PID *pcr;
    public: std::uint16_t programInfoLength;
    public: std::uint16_t serviceId;

    // ajm: ---------------------------------------------------------------------------------------
    class Entry : public Table::Entry {
      public: Entry(std::string &name, std::uint8_t **stream, Table *table);
      public: ~Entry() = default;

      std::uint16_t esInfoLength;
      PID *pid;
      std::uint8_t streamType;

      public: PMT *table;
    };
  };
}
