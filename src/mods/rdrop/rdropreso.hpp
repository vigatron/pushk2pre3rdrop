#pragma once

#include "bitptr.hpp"
#include "rdrop/rdropreso.hpp"
#include "vhmemblk.hpp"

class Reso {

public:
  Reso() {}

  verr Recover(const VHMemBlock &memblk) {
    bptr.set(memblk());
    layerscount = bptr.GByte();
    cellscnt = bptr.GByte() + 1;
    DecodeLayers();
    return vok;
  }

  void Apply(VHMemBlock &memblksrc) {
    for (int i = 0; i < 256; i++) {
      RecoverCell(memblksrc, i);
    }
  }

private:
  VHBptr bptr;
  int layerscount;
  int cellscnt;

  static const int LAYS_CNT = 16;

  static const int SIZE_META = 32;
  static const int SIZE_RESO = 256;

  uint8_t metatbl[SIZE_META * LAYS_CNT];
  uint8_t resotbl[256 * LAYS_CNT];

  // -----------------------------------------------------------------------------
  verr DecodeLayers() {
    for (int i = 0; i < layerscount; i++) {
      if (vok != DecodeLayer(i))
        return verrmsg(200, "DecodeLayers issue");
    }
    return vok;
  }

  // -----------------------------------------------------------------------------
  verr DecodeLayer(int layn) {

    uint8_t premeta[4];

    dbg.nl();
    dbg.itxt("PREMETA %d : ", layn);

    for (int i = 0; i < 4; i++) {
      premeta[i] = bptr.GByte();
      dbg.hx8(premeta[i]);
      dbg.spc();
    }

    dbg.txt("      META : ");

    // --------------------------------------------
    VHBptr bptrmeta(premeta);
    uint8_t *ptrmeta = metatbl + SIZE_META * layn;
    memset(ptrmeta, 0, SIZE_META);

    for (int i = 0; i < SIZE_META; i++) {
      bool v = bptrmeta.GBit();
      if (v) {
        ptrmeta[i] = bptr.GByte();
      }
      if (!(i % 8))
        dbg.spc();
      dbg.hx8(ptrmeta[i]);
      dbg.spc();
    }
    dbg.nl();

    // --------------------------------------------
    VHBptr bptrreso(ptrmeta);
    uint8_t *ptrreso = resotbl + SIZE_RESO * layn;
    memset(ptrreso, 0, SIZE_RESO);

    // dbg.itxt( "LAYER %d : ", layn );

    for (int i = 0; i < SIZE_RESO; i++) {
      bool v = bptrreso.GBit();
      if (v) {
        ptrreso[i] = bptr.GByte();
      }
      if (!(i % 32))
        dbg.nl();
      if (!(i % 8))
        dbg.spc();
      dbg.hx8(ptrreso[i]);
      dbg.spc();
    }
    dbg.nl();

    return vok;
  }

  // -----------------------------------------------------------------------------
  void RecoverCell(VHMemBlock &memblksrc, int celln) {
    uint8_t *ptr = memblksrc() + celln * 64;

    for (int lay = layerscount - 1; lay >= 0; lay--) {
      uint8_t upval = resotbl[celln + SIZE_RESO * lay];
      if (upval)
        for (int i = 0; i < 64; i++) {
          if (ptr[i] >= lay) {
            ptr[i] += upval;
          }
        }
    }
  }
};