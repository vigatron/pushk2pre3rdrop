#pragma once

#include <stdint.h>
#include <string>
#include <vector>

class RDBG {
public:
  RDBG();
  ~RDBG();
  void setid(uint8_t idd, std::string fname);
  void fls();
  void nwl();
  void sep();
  void hx8(uint8_t v);
  void msg(std::string msg);
  void ttt(std::string msg);
  void i1t(std::string msg, int val);
  void i2t(std::string msg, int val1, int val2);
  void i3t(std::string msg, int val1, int val2, int val3);
  void mem(const uint8_t *ptr, int length, int align = -1);
  void mem(const std::vector<uint8_t> &arr);
  void blk(const uint8_t *ptr, int w, int icnt = 32);

private:
  uint8_t id;
};

extern RDBG dbg;
