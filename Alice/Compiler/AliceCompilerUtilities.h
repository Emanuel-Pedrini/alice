#ifndef __ALICE_COMPILER_UTILITIES__
#define __ALICE_COMPILER_UTILITIES__

#include "External/Libraries.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/CodeObject.h"
#include "Headers/ArchiveManager.h"
#include "Headers/AliceTypes.h"

typedef struct StructCodeLine
{
    __AliceString* Content;
} CodeLine;

#define CLINE(L) CodeLine* L = __AlAllocate(sizeof(CodeLine));
CodeLine* CLine(const char* Content);

typedef struct StructCodeFormat
{
    __AliceString* Content;
} CodeFormat;
#define CFORMAT(F) CodeFormat* F = __AlAllocate(sizeof(CodeFormat));
#define QFORMAT(F, C) CodeFormat* F = CFormat(C);
CodeFormat* CFormat(const char* Content);

typedef struct StructCodeBlock
{
    __AliceVector* CodeLines;
    Bool HasFormat;
    CodeFormat* S;
    CodeFormat* E;
} CodeBlock;

#define CBLOCK(B) CodeBlock* B = __AlAllocate(sizeof(CodeBlock));
CodeBlock* CBlock();
Bool CBlockAppend(CodeBlock* B, CodeLine* L);
Bool CBlockFormat(CodeBlock* B, CodeFormat* S, CodeFormat* E);

typedef struct StructCodeWritter
{
    Usize Pointer;
    __AliceString* TargetPath;
    __AliceVector* WriteContent;
} CodeWritter;

#define CWRITTER(W) CodeWritter* W = __AlAllocate(sizeof(CodeWritter));
CodeWritter* CWritter(const char* TargetPath);
Bool CWriteLine(CodeWritter* W, CodeLine* L);
Bool CWriteBlock(CodeWritter* W, CodeBlock* B);
Bool CWrite(CodeWritter* W);
Bool CWriterAppendBlock(CodeWritter* W, CodeBlock* B);
Bool CWritterAppendLine(CodeWritter* W, CodeLine* L);
CodeBlock* CBlockFormatted(const char* Open, const char* Close);

#endif