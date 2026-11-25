#ifndef _AliceLLVMCompilerH_
#define _AliceLLVMCompilerH_
#include "Compiler/AliceCompilerUtilities.h"
#include "Compiler/AliceCompilerUtilities.h"
#include "Headers/AIR.h"

AliceCompilerResult* CompileToLLVMFromAIR(AliceCodeWritter* Writter, AliceIRModule* Module);

__Void CompileIRModuleFromAIR(AliceCodeWritter* Writter, AliceIRModule* Module);
__Void CompileIRFunctionFromAIR(AliceCodeWritter* Writter, AliceIRFunction* Function);
__Void CompileIRBlockFromAIR(AliceCodeWritter* Writter, AliceIRBlock* Block);
__Void CompileIRInstructionFromAIR(AliceCodeWritter* Writter, AliceIRInstruction* Instruction);
__AliceString* CompileIRValueFromAIR(AliceCodeWritter* Writter, AliceIRValue* Value);

#endif