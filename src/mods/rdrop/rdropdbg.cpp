#include "rdropdbg.hpp"

#include <fstream>
#include <iostream>
#include <string>

#define OUT_BUF_MAX 512

static char outBuf[OUT_BUF_MAX];

std::ofstream outFile;

// Initialize the file stream
static bool Init(const std::string &filename) {
  outFile.open(filename, std::ios::out | std::ios::trunc);
  if (!outFile.is_open()) {
    std::cerr << "Failed to open file: " << filename << "\n";
    return false;
  }
  return true;
}

// Write a string to the file
static void Write(const std::string &text) {
  if (outFile.is_open()) {
    outFile << text;
  } else {
    // std::cout << text;
    // std::cerr << "File is not open for writing\n";
  }
  std::cout << text;
}

// Write a char* to the file
static void Write(const char *text) {
  if (outFile.is_open()) {
    outFile << text;
  } else {
    // std::cerr << "File is not open for writing\n";
  }
  std::cout << text;
}

// Close the file explicitly
static void Close() {
  if (outFile.is_open()) {
    outFile.close();
  }
}

static const std::vector<std::string> arrhex = {"0", "1", "2", "3", "4", "5",
                                                "6", "7", "8", "9", "A", "B",
                                                "C", "D", "E", "F"};
static const std::string _nl = "\n";
static const std::string _sep = " ";

// --------------------------------------------------------------------------

RDBG::RDBG() {}

RDBG::~RDBG() { Close(); }

void RDBG::setid(uint8_t idd, std::string fname) {
  id = idd;
  Close();
  Init(fname);
}

void RDBG::fls() {}

void RDBG::nwl() { Write(_nl); }

void RDBG::sep() { Write(_sep); }

void RDBG::hx8(uint8_t v) {
  Write(arrhex[(v >> 4) & 0xF]);
  Write(arrhex[v & 0xF]);
}

void RDBG::msg(std::string msg) {
  Write(msg);
  nwl();
}

void RDBG::ttt(std::string msg) { Write(msg); }

void RDBG::i1t(std::string msg, int val) {
  snprintf(outBuf, OUT_BUF_MAX, msg.c_str(), val);
  Write(outBuf);
}

void RDBG::i2t(std::string msg, int val1, int val2) {
  snprintf(outBuf, OUT_BUF_MAX, msg.c_str(), val1, val2);
  Write(outBuf);
}

void RDBG::i3t(std::string msg, int val1, int val2, int val3) {
  snprintf(outBuf, OUT_BUF_MAX, msg.c_str(), val1, val2, val3);
  Write(outBuf);
}

void RDBG::mem(const uint8_t *ptr, int length, int align) {
  for (int i = 0; i < length; i++) {
    hx8(ptr[i]);
    if (i != (length - 1))
      sep();
  }
  if (align != -1) {
    int ins = align - length;
    for (int i = 0; i < ins; i++) {
      msg("   ");
    }
  }
}

void RDBG::mem(const std::vector<uint8_t> &arr) {
  for (int i = 0; i < arr.size(); i++) {
    if (!(i % 8))
      sep();
    hx8(arr[i]);
    sep();
  }
}

void RDBG::blk(const uint8_t *ptr, int w, int icnt) {
  int rows = (w / icnt) + ((w % icnt) ? 1 : 0);
  for (int j = 0; j < rows; j++) {
    int offs = j * icnt;
    int cnt = (j != (rows - 1)) ? icnt : (w - offs);
    i2t("%8d %8X : ", offs, offs);
    for (int i = 0; i < cnt; i++) {
      uint8_t v = ptr[offs + i];
      if (!(i % 8)) {
        sep();
      }
      i1t("%.2x ", v);
    }
    nwl();
  }
  fls();
}
