#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "rdroplayer.hpp"
#include "rdropmeta.hpp"
#include "rdropmetahdr.hpp"
#include "rdropsqueeze.hpp"

// Logger ID
#define DBGMOD_RDRP 1
#include "rdropdbg.hpp"

// ---------------------------------------

class VHRDropLayers {

public:
  // -----------------------------------------------------------------------------------
  VHRDropLayers() {}

  // -----------------------------------------------------------------------------------
  bool Process(std::vector<uint8_t> srcblock, uint16_t qwdt, uint8_t laysmax,
               std::string fnamelog) {

    // Setup logger
    dbg.setid(DBGMOD_RDRP, fnamelog);

    artefact_bin = ProcessAllCells(srcblock, qwdt, laysmax);
    DumpTable();

    artefact_rtbl = AssembleTable();
    dbg.i1t("Final RDropblock size %d bytes\n\n", artefact_rtbl.size());

    return true;
  }

  // -----------------------------------------------------------------------------------
  std::vector<uint8_t> getResultBin() { return artefact_bin; }

  // -----------------------------------------------------------------------------------
  std::vector<uint8_t> getResultTbl() { return artefact_rtbl; }

  // -----------------------------------------------------------------------------------
  int nonEmptyLayersCount() {
    for (int i = LAY_COUNT_MAX; i; i--) {
      if (!premeta[i - 1].empty())
        return i;
    }
    return 0;
  }

  // -----------------------------------------------------------------------------------
  // Generate metatable & meta prefix
  // -----------------------------------------------------------------------------------
  void GenerateMetaAndPremeta() {
    for (int i = 0; i < LAY_COUNT_MAX; i++) {
      meta[i].GenerateMetaLayer(droptbl[i].data());
      premeta[i].set(meta[i].getarr());
    }
  }

  // -----------------------------------------------------------------------------------
  // Layers content
  // -----------------------------------------------------------------------------------
  std::vector<uint8_t> AssembleLayer(int ll) {

    std::vector<uint8_t> rarr;
    rarr.reserve(4 + 32 + 256);

    // write meta-hdr bytes
    const std::array<uint8_t, 4> &metahdr = premeta[ll].getarr();
    rarr.insert(rarr.end(), metahdr.begin(), metahdr.end());

    // write nonzero meta values
    const std::array<uint8_t, 32> &metaarr = meta[ll].getarr();
    for (int i = 0; i < 32; i++) {
      if (metaarr[i]) {
        rarr.push_back(metaarr[i]);
      }
    }

    // write nonzero layer values
    const std::array<uint8_t, 256> &layerarr = droptbl[ll].data();
    for (int i = 0; i < 256; i++) {
      if (layerarr[i]) {
        rarr.push_back(layerarr[i]);
      }
    }

    return rarr;
  }

  // -----------------------------------------------------------------------------------
  std::vector<uint8_t> AssembleTable() {

    std::vector<uint8_t> rarr;

    // rdrop hdr
    int laycnt = nonEmptyLayersCount();
    rarr.push_back(laycnt);

    // rarr.push_back(255); // count is part of cfg ( offset cnt blksize)

    // rdrop layerhdr + layermeta + layers ( skipping zeros )
    for (int ll = 0; ll < laycnt; ll++) {
      std::vector<uint8_t> r = AssembleLayer(ll);
      rarr.insert(rarr.end(), r.begin(), r.end());
    }
    return rarr;
  }

  // -----------------------------------------------------------------------------------
  bool SaveRDropLayersAsSingleFiles(std::string basefname) {
    int laycnt = nonEmptyLayersCount();
    for (int i = 0; i < laycnt; i++) {
      std::vector<uint8_t> vec(droptbl[i].data().begin(),
                               droptbl[i].data().end());
      std::string fname = basefname + "_tbl_" + std::to_string(i) + ".bin";
      if (!writeVectorToFile(fname, vec))
        return false;
    }
    return true;
  }

  // -----------------------------------------------------------------------------------
  bool writeVectorToFile(const std::string &filename,
                         const std::vector<uint8_t> &data) {
    namespace fs = std::filesystem;

    fs::path filePath(filename);
    fs::path dir = filePath.parent_path();

    // Create directory if it does not exist
    if (!dir.empty() && !fs::exists(dir)) {
      if (!fs::create_directories(dir)) {
        return false; // Failed to create directory
      }
    }

    // Open file in binary mode
    std::ofstream out(filePath, std::ios::binary);
    if (!out) {
      return false; // Failed to open file
    }

    // Write data to file
    out.write(reinterpret_cast<const char *>(data.data()),
              static_cast<std::streamsize>(data.size()));
    if (!out) {
      return false; // Write error
    }

    return true; // Success
  }

private:
  // logger
  RDBG dbg;
  //
  std::vector<uint8_t> artefact_bin;
  std::vector<uint8_t> artefact_rtbl;

  //
  static const int LAY_COUNT_MAX = 16;

  // 4  Bytes  : Short meta per layer
  VHPreMeta premeta[LAY_COUNT_MAX];

  // 32 Bytes  : Meta слой понижения разрядности
  VHRDropMeta meta[LAY_COUNT_MAX];

  // 256 Bytes : Reso Drop table
  VHRDropLayer droptbl[LAY_COUNT_MAX];

  uint8_t misc_symreso(uint8_t sym) {
    return (!sym) ? 1 : (8 - __builtin_clz(sym));
  }

  // Max resolution of the block
  uint8_t misc_blkreso(const uint8_t *ptr, uint16_t len) {
    uint8_t rmax = 0;
    for (int i = 0; i < len; i++) {
      uint8_t r = misc_symreso(ptr[i]);
      if (r > rmax)
        rmax = r;
    }
    return rmax ? rmax : 1;
  }

  //
  void clear() {
    for (int i = 0; i < LAY_COUNT_MAX; i++)
      droptbl[i].clear();
  }

  // -----------------------------------------------------------------------------------
  // 8x8=64 pixels in cell ( blen = 64 as default )
  // -----------------------------------------------------------------------------------
  std::vector<uint8_t> ProcessAllCells(std::vector<uint8_t> arrbin,
                                       uint16_t cellwdt, uint8_t laysmax) {

    dbg.msg("ResoDrop : ProcessAllCells() ");

    clear();

    std::vector<uint8_t> r;
    r.reserve(arrbin.size());

    // Total cells count
    int blkcnt = arrbin.size() / cellwdt;
    int laycnt = VHRDropLayers::LAY_COUNT_MAX;

    // Process all cells
    for (int celln = 0; celln < blkcnt; celln++) {

      // Copy cell content
      int start = celln * cellwdt;
      std::vector<uint8_t> arrcell(arrbin.begin() + start,
                                   arrbin.begin() + start + cellwdt);

      for (int i = 0; i < laycnt; i++) {

        SqueezeRange blk(arrcell);

        dbg.i1t("Cell #%.3d   ", celln);
        dbg.i1t("satcnt=%.3d  ", blk.count());
        dbg.mem(blk.ptrsorted(), blk.count());
        dbg.nwl();

        int dropidx;
        int depth = (blk.count() > laycnt) ? laycnt : blk.count();
        bool dropflag = !blk.islinear(depth, &dropidx);

        if (dropflag) {
          uint8_t *ptr = arrcell.data();
          blk.ApplyColorDrop(ptr, cellwdt, dropidx);
          droptbl[dropidx].set(celln, blk.delta());
        } else {
          i = laycnt;
        }
      }
      r.insert(r.end(), arrcell.begin(), arrcell.end());
    }

    dbg.nwl();
    GenerateMetaAndPremeta();
    dbg.nwl();

    return r;
  }

  // -----------------------------------------------------------------------------------
  void DumpTable() {

    int laycnt = nonEmptyLayersCount();

    dbg.msg("Color & Resolution drop tables:");
    dbg.i1t("Layers count %d", laycnt);
    dbg.nwl();

    for (int i = 0; i < laycnt; i++) {

      std::vector<uint8_t> vec1(premeta[i].getarr().begin(),
                                premeta[i].getarr().end());

      std::vector<uint8_t> vec2(meta[i].getarr().begin(),
                                meta[i].getarr().end());

      int bytes_pre = 4;
      int bytes_meta = meta[i].nonzerocnt();
      int bytes_dtbl = droptbl[i].nonzerocnt();

      VHRDropLayer &t = droptbl[i];
      uint8_t reso = misc_blkreso(t.ptr(), t.size());
      dbg.i2t("LAY #%d  R[%d]  ", i, reso);

      dbg.i3t(" %d + %2d + %3d  ", bytes_pre, bytes_meta, bytes_dtbl);
      dbg.i1t(" = %3d Bytes    ", bytes_pre + bytes_meta + bytes_dtbl);

      dbg.mem(vec1);
      dbg.ttt("   ");
      dbg.mem(vec2);
      dbg.nwl();
    }

    dbg.nwl();

    for (int i = 0; i < laycnt; i++) {
      VHRDropLayer &t = droptbl[i];
      uint8_t reso = misc_blkreso(t.ptr(), t.size());
      dbg.i1t("Table #%d ", i);
      dbg.i2t(" R[%d] Marked=%d\n", reso, meta[i].CalcCount());
      dbg.blk(t.ptr(), 256);
      dbg.nwl();
    }

    dbg.fls();
  }
};
