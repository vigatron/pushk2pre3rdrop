#pragma once

#include <algorithm>
#include <array>
#include <cstdint>

class VHRDropLayer {
public:
  VHRDropLayer() { clear(); }

  void clear() { arr.fill(0); }

  uint8_t &operator[](uint8_t idx) { return arr[idx]; }

  uint8_t operator[](uint8_t idx) const { return arr[idx]; }

  void set(int idx, uint8_t v) { arr[idx] = v; }

  uint16_t nonzerocnt() const {
    return std::count_if(arr.begin(), arr.end(),
                         [](uint8_t v) { return v != 0; });
  }

  const uint8_t *ptr() const { return arr.data(); }

  int size() const { return arr.size(); }

  const std::array<uint8_t, 256> &data() const { return arr; }

private:
  std::array<uint8_t, 256> arr{};
};
