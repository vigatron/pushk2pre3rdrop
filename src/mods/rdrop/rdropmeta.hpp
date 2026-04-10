#pragma once

#include <array>
#include <stdint.h>

// -----------------------------------------------------------------------------------
// 32 bytes metadata
// -----------------------------------------------------------------------------------

class VHRDropMeta {

public:
  // -----------------------------------------------------------------------------------
  int CalcCount() {
    int r = 0;
    uint8_t msk = 0x80;
    uint8_t pos = 0;
    for (int i = 0; i < 256; i++) {
      if (arr[pos] & msk) {
        r++;
      }
      msk >>= 1;
      if (!msk) {
        msk = 0x80;
        pos++;
      }
    }
    return r;
  }

  // -----------------------------------------------------------------------------------
  uint8_t nonzerocnt() {
    uint8_t r = 0;
    for (int i = 0; i < 32; i++)
      if (arr[i])
        r++;
    return r;
  }

  // -----------------------------------------------------------------------------------
  bool operator[](int i) const {
    int byten = i >> 3;
    int mask = 0x80 >> (i % 8);
    return (arr[byten] & mask) > 0;
  }

  // -----------------------------------------------------------------------------------
  uint8_t byteval(int idx) const { return arr[idx]; }

  // -----------------------------------------------------------------------------------
  void GenerateMetaLayer(const std::array<uint8_t, 256> &dtbl) {

    uint8_t msk = 0x80;
    uint8_t pos = 0;
    for (int i = 0; i < 256; i++) {
      if (dtbl[i])
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

  const std::array<uint8_t, 32> &getarr() { return arr; }

private:
  std::array<uint8_t, 32> arr{};
};
