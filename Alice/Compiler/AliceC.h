#ifndef __ALICE_C_COMPILER__
#define __ALICE_C_COMPILER__

#include "Headers/AliceTypes.h"
#include "External/Libraries.h"
#include "Headers/InterpreterTypes.h"
#include "Headers/StringUtilities.h"
#include "Headers/DictionaryUtilities.h"
#include "Headers/CodeObject.h"
#include "Headers/ArchiveManager.h"
#include "AliceCompilerUtilities.h"

CodeBlock* CLang_Include(const char* LibName);
CodeLine* CLang_InitVariable(const char* Name, AliceType* Type);
CodeLine* CLang_NonInitVariable(const char* Name, AliceType* Type);
CodeBlock* CLang_Function(const char* Name, Vector* Args, CodeBlock* Body, AliceType* RType);
CodeBlock* CLang_Main(CodeBlock* Body);
String* AliceTypeToCType(AliceType* T);
#endif