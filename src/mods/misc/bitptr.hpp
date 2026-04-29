#pragma once

#include "vhplatform.hpp"

class VHBptr {
public:
  VHBptr() {}
  VHBptr(u8 *pa) { set(pa); }
  void set(u8 *pa) {
    ptr = pa;
    bcnt = 0;
  }
  void inc() { bcnt++; }
  void dec() { bcnt--; }
  u8 *addr() { return ptr + (bcnt >> 3); }
  u8 msk0() { return mask0[bcnt & 0x7]; }
  u8 msk1() { return mask1[bcnt & 0x7]; }

  void PBit0() {
    *addr() &= msk0();
    inc();
  }
  void PBit1() {
    *addr() |= msk1();
    inc();
  }
  void PBit(u8 b) {
    if (b)
      PBit1();
    else
      PBit0();
  }
  void PHalf(u8 v) {
    for (u8 m = 0x08; m; m >>= 1) {
      PBit(v & m);
    }
  }
  void PByte(u8 v) {
    for (u8 m = 0x80; m; m >>= 1) {
      PBit(v & m);
    }
  }

  u8 TBit() { return *addr() & msk1() ? 1 : 0; }

  u8 GBit() {
    u8 r = TBit();
    inc();
    return r;
  }
  u8 G2Bits() {
    u8 r = (GBit() << 1) + GBit();
    return r;
  }
  u8 G3Bits() {
    u8 r = (GBit() << 2) + G2Bits();
    return r;
  }
  u8 GHalf() {
    u8 r = 0, c = 4;
    while (c--) {
      r <<= 1;
      r |= GBit();
    }
    return r;
  }
  u8 G5Bits() {
    u8 r = (GBit() << 4) + GHalf();
    return r;
  }
  u8 G6Bits() {
    u8 r = (G2Bits() << 4) + GHalf();
    return r;
  }
  u8 G7Bits() {
    u8 r = (G3Bits() << 4) + GHalf();
    return r;
  }
  u8 GByte() {
    u8 r = 0, c = 8;
    while (c--) {
      r <<= 1;
      r |= GBit();
    }
    return r;
  }
  u16 GWord() {
    u16 r1 = GByte() << 8, r2 = GByte();
    return r1 | r2;
  }

  u16 bytescnt() { return (bcnt >> 3) + lastbyte(); }
  u8 lastbyte() { return ((bcnt & 7) ? 1 : 0); }

private:
  const u8 mask0[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};
  const u8 mask1[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
  u16 bcnt;
  u8 *ptr;
};