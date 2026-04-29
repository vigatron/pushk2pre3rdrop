#include "argsparser.hpp"
#include "errors.hpp"
#include "misc/fileio.hpp"
#include "rdrop/rdroplayers.hpp"

//-----------------------------------------------------------
int func_split(int argc, char *argv[]) {

  if (argc != 8)
    return eError_InvalidParams;

  std::string fname_inp(argv[2]);
  if (!fileExists(fname_inp))
    return eError_InvalidFile_Input;

  std::string fname_tbl(argv[3]);
  std::string fname_out(argv[4]);

  bool parseok;

  int offs = parseArgInt(argv[5], parseok);
  if (!parseok) {
    return eError_InvalidParams;
  }

  int count = parseArgInt(argv[6], parseok);
  if (!parseok) {
    return eError_InvalidParams;
  }

  int bsize = parseArgInt(argv[7], parseok);
  if (!parseok) {
    return eError_InvalidParams;
  }

  // Load source file
  std::vector<uint8_t> arrsrc = load_binary_file(fname_inp);

  if (!arrsrc.size())
    return eError_EmptySourceFile;

  int volume = count * bsize;
  int bounds = offs + volume;

  if (arrsrc.size() < bounds)
    return eError_BoundsCheckFailed;

  // DRop params : Offset Count CellSize

  // Process RDrop
  VHRDropLayers rdrp;

  // Copy source block
  int LAYERS_MAX = 16;

  std::string file_log = fname_out + ".rdrop_log.txt";

  if (!rdrp.Process(arrsrc, bsize, 16, file_log)) {
    return eError_ProcessError;
  }

  if (!rdrp.writeVectorToFile(fname_out, rdrp.getResultBin())) {
    return eError_CantWrite_Outfile;
  }

  if (!rdrp.writeVectorToFile(fname_tbl, rdrp.getResultTbl())) {
    return eError_CantWrite_Outtbl;
  }

  // Store original unpacked tables
  if (!rdrp.SaveRDropLayersAsSingleFiles(fname_out))
    return false;

  return eOk;
}
