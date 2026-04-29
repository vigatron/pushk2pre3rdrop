#include "vhplatform.hpp"

#ifdef APP_NAME_STR
const std::string APP_NAME = APP_NAME_STR;
#else
const std::string APP_NAME = "pushk2pre3rdrop";
#endif

#ifndef APP_VERSION
#define APP_VERSION 0
#endif

#include "argsparser.hpp"
#include "misc/errors.hpp"
#include "misc/fileio.hpp"
#include "rdrop/rdroplayers.hpp"

int func_split(int argc, char *argv[]);
int func_merge(int argc, char *argv[]);

//-----------------------------------------------------------
int main(int argc, char *argv[]) {

  if (argc < 2) {
    std::cout << APP_NAME << " : no input parameters" << std::endl;
    return eError_NoParams;
  }

  std::string cmd(argv[1]);

  if (cmd == "v") {
    std::cout << "AppName: " << APP_NAME << " ver " << APP_VERSION << std::endl;
    return eOk;
  } else if (cmd == "s") {
    return func_split(argc, argv);
  } else if (cmd == "m") {
    return func_merge(argc, argv);
  }

  std::cout << "Invalid command : " << cmd << std::endl;

  return eError_InvalidCommand;
}