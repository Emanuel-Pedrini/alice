#ifndef __ALICE_COMPILER_UTILITIES__
#define __ALICE_COMPILER_UTILITIES__

#include "External/Libraries.h"
#include "Headers/InterpreterTypes.h"
#include "Headers/StringUtilities.h"
#include "Headers/DictionaryUtilities.h"
#include "Headers/CodeObject.h"
#include "Headers/ArchiveManager.h"

typedef struct StructCodeLine
{
    String* Content;
} CodeLine;

#define CLINE(L) CodeLine* L = malloc(sizeof(CodeLine));
CodeLine* CLine(const char* Content);

typedef struct StructCodeFormat
{
    String* Content;
} CodeFormat;
#define CFORMAT(F) CodeFormat* F = malloc(sizeof(CodeFormat));
#define QFORMAT(F, C) CodeFormat* F = CFormat(C);
CodeFormat* CFormat(const char* Content);

typedef struct StructCodeBlock
{
    Vector* CodeLines;
    Bool HasFormat;
    CodeFormat* S;
    CodeFormat* E;
} CodeBlock;

#define CBLOCK(B) CodeBlock* B = malloc(sizeof(CodeBlock));
CodeBlock* CBlock();
Bool CBlockAppend(CodeBlock* B, CodeLine* L);
Bool CBlockFormat(CodeBlock* B, CodeFormat* S, CodeFormat* E);

typedef struct StructCodeWritter
{
    Usize Pointer;
    String* TargetPath;
    Vector* WriteContent;
} CodeWritter;

#define CWRITTER(W) CodeWritter* W = malloc(sizeof(CodeWritter));
CodeWritter* CWritter(const char* TargetPath);
Bool CWriteLine(CodeWritter* W, CodeLine* L);
Bool CWriteBlock(CodeWritter* W, CodeBlock* B);
Bool CWrite(CodeWritter* W);
Bool CWriterAppendBlock(CodeWritter* W, CodeBlock* B);
Bool CWritterAppendLine(CodeWritter* W, CodeLine* L);
CodeBlock* CBlockFormatted(const char* Open, const char* Close);
#endif