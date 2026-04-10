#pragma once

#include <array>
#include <stdint.h>

// -----------------------------------------------------------------------------------
// 4 bytes metahdr
// -----------------------------------------------------------------------------------

class VHPreMeta {

public:
  void set(const std::array<uint8_t, 32> &arrmeta) {

    uint8_t msk = 0x80;
    uint8_t pos = 0;
    for (int i = 0; i < 32; i++) {
      if (arrmeta[i])
        arr[pos] |= msk;
      else
        arr[pos] &= ~msk;
      msk >>= 1;
      if (!msk) {
        msk = 0x80;
        pos++;
      }
    }
  }

  bool empty() const {
    bool is_empty = (arr == std::array<uint8_t, 4>{});
    return is_empty;
  }

  const std::array<uint8_t, 4> &getarr() { return arr; }

private:
  std::array<uint8_t, 4> arr{};
};
