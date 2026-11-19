#ifndef __ALICE_C_COMPILER__
#define __ALICE_C_COMPILER__

#include "Headers/AliceTypes.h"
#include "External/Libraries.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/CodeObject.h"
#include "Headers/ArchiveManager.h"
#include "AliceCompilerUtilities.h"

CodeBlock* CLang_Include(const char* __LibName);
CodeLine* CLang_InitVariable(const char* __Name, const __AliceType* __Type, CodeBlock* __Block);
CodeBlock* CLang_Function(const char* __Name, __AliceVector* __Arguments, const __AliceType* __ReturnType);
CodeBlock* CLang_Main();
__AliceString* AliceTypeToCType(const __AliceType* __Type);
#endif