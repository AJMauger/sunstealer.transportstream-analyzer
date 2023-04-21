#pragma once

namespace ajm::iso13818_1 {
  // ajm: -----------------------------------------------------------------------------------------
  class Table : public ajm::system::Object {
    public: Table(const std::string &name, Section *section, std::uint8_t tableId);
    public: virtual ~Table();

    public: virtual void OnSection()=0;

    // ajm: ---------------------------------------------------------------------------------------
    class Entry : public ajm::system::Object {
      public: Entry(const std::string &name, Table *table);
      public: virtual ~Entry();

      public: std::vector<std::shared_ptr<Descriptor>> descriptors;
      public: Table *table;
    };

    public: std::vector<std::shared_ptr<Descriptor>> descriptors;
    public: std::vector<std::shared_ptr<Entry>> entries;
    public: Section *section;
    public: std::map<std::uint32_t, std::shared_ptr<std::uint8_t>> sections; // ajm: => std::uint32_t sections;
    public: std::uint8_t tableId;
    public: std::uint8_t versionNumber;
  };
}
