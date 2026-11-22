#ifndef _AliceLLVMCompilerH_
#define _AliceLLVMCompilerH_
#include "Compiler/AliceCompilerUtilities.h"
#include "Headers/ForwardInclude.h"

AliceCompilerResult* CompileToLLVMFromAIR(AliceCodeWritter* Writter, AliceIRModule* Module);

#endif