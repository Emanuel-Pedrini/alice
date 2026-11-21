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
} __AliceCodeLine;

#define CLINE(L) __AliceCodeLine* L = __AlAllocate(sizeof(__AliceCodeLine));
__AliceCodeLine* CLine(const char* Content);

typedef struct StructCodeFormat
{
    __AliceString* Content;
} __AliceCodeFormat;
#define CFORMAT(F) __AliceCodeFormat* F = __AlAllocate(sizeof(__AliceCodeFormat));
#define QFORMAT(F, C) __AliceCodeFormat* F = CFormat(C);
__AliceCodeFormat* CFormat(const char* Content);

typedef struct StructCodeBlock
{
    __AliceVector* CodeLines;
    Bool HasFormat;
    __AliceCodeFormat* S;
    __AliceCodeFormat* E;
} __AliceCodeBlock;

#define CBLOCK(B) __AliceCodeBlock* B = __AlAllocate(sizeof(__AliceCodeBlock));
__AliceCodeBlock* CBlock();
Bool CBlockAppend(__AliceCodeBlock* B, __AliceCodeLine* L);
Bool CBlockFormat(__AliceCodeBlock* B, __AliceCodeFormat* S, __AliceCodeFormat* E);

typedef struct StructCodeWritter
{
    Usize Pointer;
    __AliceString* TargetPath;
    __AliceVector* WriteContent;
} CodeWritter;

#define CWRITTER(W) CodeWritter* W = __AlAllocate(sizeof(CodeWritter));
CodeWritter* CWritter(const char* TargetPath);
Bool CWriteLine(CodeWritter* W, __AliceCodeLine* L);
Bool CWriteBlock(CodeWritter* W, __AliceCodeBlock* B);
Bool CWrite(CodeWritter* W);
Bool CWriterAppendBlock(CodeWritter* W, __AliceCodeBlock* B);
Bool CWritterAppendLine(CodeWritter* W, __AliceCodeLine* L);
__AliceCodeBlock* CBlockFormatted(const char* Open, const char* Close);

#endif