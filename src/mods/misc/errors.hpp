#pragma once

enum enError {
  eOk = 0,
  eUnsupported,
  eError_NoParams,
  eError_InvalidCommand,
  eError_InvalidParams,
  eError_InvalidFile_Input,
  eError_InvalidFile_rtbl,
  eError_InvalidFile_Output,
  eError_EmptySourceFile,
  eError_BoundsCheckFailed,
  eError_ProcessError,
  eError_CantWrite_Outfile,
  eError_CantWrite_Outtbl,
};
