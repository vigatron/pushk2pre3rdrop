#include "vhplatform.hpp"

//-----------------------------------------------------------
verr verrmsg(int x, std::string strerr) {
  std::cout << "\nERROR : " << strerr << std::endl;
  return x;
}
