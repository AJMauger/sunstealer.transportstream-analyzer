#pragma once

namespace ajm::iso13818_1 {
  class PMT;

  // ajm: -----------------------------------------------------------------------------------------
  class PAT : public Table {
    public: explicit PAT(Section *section); 
    public: virtual ~PAT() = default;

    public: PMT* GetPMT(std::uint16_t serviceId);
    public: void Debug();
    public: void OnSection();

    // ajm: ---------------------------------------------------------------------------------------
    class Entry : public Table::Entry {
      public: Entry(std::string &name, std::uint8_t **stream, PAT *table);
      public: ~Entry() = default;

      public: PID *pid;
      public: PMT *pmt;
      public: std::uint16_t serviceId;
    };
  };
}