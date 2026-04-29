#include "argsparser.hpp"
#include "errors.hpp"
#include "misc/fileio.hpp"

// #include "rdrop/rdroplayers.hpp"
#include "rdrop/rdropreso.hpp"

#include "vhmemblk.hpp"
#include "vhplatform.hpp"

//-----------------------------------------------------------
int func_merge(int argc, char *argv[]) {

  bool parseok;

  if (argc != 8) {
    return verrmsg(eError_InvalidParamsCount, "func_merge, argc count");
  }

  // Check source file
  std::string fname_inp(argv[2]);
  if (!fileExists(fname_inp))
    return eError_InvalidFile_Input;

  // Check rtable file
  std::string fname_tbl(argv[3]);
  if (!fileExists(fname_tbl))
    return eError_InvalidFile_rtbl;

  // Result / destination
  std::string fname_out(argv[4]);

  // Starting offset
  int offs = parseArgInt(argv[5], parseok);
  if (!parseok) {
    return eError_InvalidParams;
  }

  // Count
  int count = parseArgInt(argv[6], parseok);
  if (!parseok) {
    return eError_InvalidParams;
  }

  // Block length
  int blen = parseArgInt(argv[7], parseok);
  if (!parseok) {
    return eError_InvalidParams;
  }

  // Parameters parsed, starting recovery process

  VHMemBlock memblksrc;
  if (!memblksrc.LoadFromFile(fname_inp.c_str()))
    return verrmsg(eError_InvalidFile_Input, "source file read issue");

  VHMemBlock memblkreso;
  if (!memblkreso.LoadFromFile(fname_tbl.c_str()))
    return verrmsg(eError_InvalidFile_rtbl, "reso table read issue");

  Reso reso;
  reso.RestoreTable(memblkreso);
  reso.Apply(memblksrc, count, blen);

  if (!memblksrc.SaveToFile(fname_out))
    return verrmsg(eError_CantWrite_Outfile, "save results issue");

  return eOk;
}
