#pragma once

#include <array>
#include <stdint.h>
#include <vector>

class SqueezeRange {

public:
  SqueezeRange(const std::vector<uint8_t> &arr) {

    std::array<uint8_t, 256> bytemap{};

    // Mark presence
    for (uint8_t v : arr) {
      bytemap[v] = 1;
    }

    // Collect sorted unique values
    cnt = 0;
    for (size_t i = 0; i < bytemap.size(); ++i) {
      if (bytemap[i]) {
        sorted[cnt++] = static_cast<uint8_t>(i);
      }
    }
  }

  // -----------------------------------------------------------------------------------
  bool islinear(uint8_t depth, int *ridx) {
    for (int i = 0; i < depth; i++)
      if (sorted[i] != i) {
        *ridx = i;
        return false;
      }
    return true;
  }

  // -----------------------------------------------------------------------------------
  int count() { return cnt; }

  // -----------------------------------------------------------------------------------
  uint8_t *ptrsorted() { return sorted; }

  // -----------------------------------------------------------------------------------
  void ApplyColorDrop(uint8_t *ptr, int sz, uint8_t dropidx) {
    uint8_t srcval = sorted[dropidx];
    dv = srcval - dropidx;
    for (int i = 0; i < sz; i++) {
      if (*ptr >= dropidx)
        *ptr -= dv;
      ptr++;
    }
  }

  uint8_t delta() { return dv; }

private:
  std::vector<uint8_t> src;

  // Valid colors mask in cell
  int cnt;
  uint8_t dv;
  uint8_t sorted[256];
};
