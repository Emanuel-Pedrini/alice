#ifndef __ALICE_COMPILER_UTILITIES__
#define __ALICE_COMPILER_UTILITIES__

#include "External/Libraries.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/CodeObject.h"
#include "Headers/ArchiveManager.h"
#include "Headers/AliceTypes.h"
#include "Headers/ForwardInclude.h"

typedef enum Enum__AliceCodeContextKind
{
    AlContext_Line = -1,
    AlContext_Block = 1
} AliceCodeContextKind;

struct Struct__AliceCompilerResult
{
    Bool Ok;
    __AliceString* RLiteral;
};

union Union__AliceCodeContent
{
    AliceCodeLine* Line;
    AliceCodeBlock* Block;
};

struct Struct__AliceCodeContext
{
    AliceCodeContextKind Kind;
    AliceCodeContent Content;
};

struct Struct__AliceCodeLine
{
    __AliceString* Content;
};

struct Struct__AliceCodeFormat
{
    __AliceString* Content;
};

struct Struct__AliceCodeBlock
{
    __AliceVector* CodeLines; // Vector<AliceCodeContext*>*
    Usize LineCount;
    Usize BlockCount;
    Bool HasFormat;
    AliceCodeFormat* OpenFormat;
    AliceCodeFormat* CloseFormat;
    AliceCodeFormat* LineFormat;
};

struct Struct__AliceCodeWritter
{
    Usize Pointer;
    __AliceString* TargetPath;
    __AliceVector* WriteContent;
    AliceCodeBlock* CurrentBlock;
};

AliceCodeWritter* NewAliceCodeWritter(CString TargetPath);
AliceCodeLine* NewCodeLine(CString Literal);
AliceCodeBlock* NewCodeBlock();
AliceCodeContext* NewCodeContext(AliceCodeContextKind Kind, __VoidPointer Content);
AliceCodeFormat* NewCodeFormat(CString Literal);
AliceCompilerResult* NewAliceCompilerResult(Bool Ok, __AliceString* Literal);

Bool NewCodeBlockLine(AliceCodeBlock* Block, AliceCodeLine* Line);
Bool NewCodeBlockBlock(AliceCodeBlock* Block, AliceCodeBlock* OtherBlock);
Bool NewCodeWriterBlock(AliceCodeWritter* Writter, AliceCodeBlock* Block);
Bool New__AliceCodeWritterLine(AliceCodeWritter* Writter, AliceCodeLine* Line);
AliceCodeBlock* NewCodeBlockFormatted(CString OpenFormat, CString CloseFormat);
Bool GetCompilationWasSucessfull(AliceCompilerResult* Result);
__AliceString* CodeGenerateCodeWritter(AliceCodeWritter* Writter);
__AliceString* GetCodeByContext(AliceCodeWritter* Writter, AliceCodeContext* Context);
Bool CodeWrite(AliceCodeWritter* Writter);
#endif