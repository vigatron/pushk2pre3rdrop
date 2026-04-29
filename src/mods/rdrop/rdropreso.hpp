#pragma once

#include "vhplatform.hpp"

#include "bitptr.hpp"
#include "rdrop/rdropdbg.hpp"
#include "rdrop/rdropreso.hpp"
#include "vhmemblk.hpp"

class Reso {

public:
  Reso() {}

  verr RestoreTable(const VHMemBlock &memblkrtbl) {
    bptr.set(memblkrtbl());
    layerscount = bptr.GByte();
    DecodeLayers();
    return vok;
  }

  void Apply(VHMemBlock &memblksrc, int cnt, int blen) {
    for (int i = 0; i < cnt; i++) {
      RecoverCell(memblksrc, i, blen);
    }
  }

private:
  VHBptr bptr;
  int layerscount;
  //   int cellscnt;
  //     cellscnt = cnt;

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

    dbg.nwl();
    dbg.i1t("PREMETA %d : ", layn);

    for (int i = 0; i < 4; i++) {
      premeta[i] = bptr.GByte();
      dbg.hx8(premeta[i]);
      dbg.sep();
    }

    dbg.msg("      META : ");

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
        dbg.sep();
      dbg.hx8(ptrmeta[i]);
      dbg.sep();
    }
    dbg.nwl();

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
        dbg.nwl();
      if (!(i % 8))
        dbg.sep();
      dbg.hx8(ptrreso[i]);
      dbg.sep();
    }
    dbg.nwl();

    return vok;
  }

  // -----------------------------------------------------------------------------
  void RecoverCell(VHMemBlock &memblksrc, int celln, int blen) {
    uint8_t *ptr = memblksrc() + celln * blen;

    for (int lay = layerscount - 1; lay >= 0; lay--) {
      uint8_t upval = resotbl[celln + SIZE_RESO * lay];
      if (upval)
        for (int i = 0; i < blen; i++) {
          if (ptr[i] >= lay) {
            ptr[i] += upval;
          }
        }
    }
  }
};