#include <sstream>

int parseArgInt(const char *arg, bool &ok) {
  std::stringstream ss(arg);
  int value;
  ss >> value;
  ok = !ss.fail() && ss.eof();
  return value;
}