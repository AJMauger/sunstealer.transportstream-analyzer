#pragma once

namespace ajm::iso13818_1 {
  // ajm: -----------------------------------------------------------------------------------------
  class CAT : public Table {
    public: explicit CAT(Section *section);
    public: virtual ~CAT() = default;

    public: virtual void Debug() {};
    public: void OnSection();
  };
}