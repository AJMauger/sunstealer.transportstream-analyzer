#pragma once

namespace ajm::system::math {

  void GetFactors(std::int32_t i) {
    std::printf("GetFactors(%d).\n", i);
    for (std::int32_t j=1; j<i+1; j++) {
      if ((i % j)==0) {
        std::printf("factor: %d.\n", j);
      }
    }
  }
}