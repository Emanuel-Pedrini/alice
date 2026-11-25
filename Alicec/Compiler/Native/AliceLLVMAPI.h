#ifndef _AliceLLVMCompilerAPIH_
#define _AliceLLVMCompilerAPIH_
#include "Compiler/AliceCompilerUtilities.h"
#include "Headers/ForwardInclude.h"

int GenerateObjectFileFromLLVMIR(CString LLVMIR, CString OutputPath, CString Passes, Bool SpecificCpu);
int GenerateExecutableFromObjectFile(CString LLVMIR, CString OutputPath, CString Passes, Bool SpecificCpu, Bool KeepTemporaries);

#endif