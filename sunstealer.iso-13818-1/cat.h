#pragma once

namespace ajm::iso13818_1 {
  // ajm: -----------------------------------------------------------------------------------------
  class CAT : public Table {
    public: CAT(Section *section);
    public: virtual ~CAT() {}

    public: virtual void Debug() {};
    public: void OnSection();
  };
}