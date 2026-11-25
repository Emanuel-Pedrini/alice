#include "Compiler/AliceCompilerUtilities.h"
AliceCodeWritter* NewAliceCodeWritter(CString TargetPath)
{
    AliceCodeWritter* CWritter = __AlAllocate(sizeof(AliceCodeWritter));
    CWritter -> Pointer = 0;
    CWritter -> TargetPath = __AlStringFrom(TargetPath);
    CWritter -> WriteContent = __NewAlVector(sizeof(AliceCodeBlock));
    CWritter -> CurrentBlock = NewCodeBlock();
    return CWritter;
}

AliceCodeContext* NewCodeContext(AliceCodeContextKind Kind, __VoidPointer Content)
{
    AliceCodeContext* CContext = __AlAllocate(sizeof(AliceCodeContext));
    CContext -> Kind = Kind;
    switch (CContext -> Kind)
    {
        case AlContext_Line: { CContext->Content.Line = ((AliceCodeLine*)Content); break;}
        case AlContext_Block: { CContext->Content.Block = ((AliceCodeBlock*)Content); break;}
    }
    return CContext;
}

AliceCodeLine* NewCodeLine(CString Literal)
{
    AliceCodeLine* CLine = __AlAllocate(sizeof(AliceCodeLine));
    CLine -> Content = __AlStringFrom(Literal);
    return CLine;
}

AliceCodeBlock* NewCodeBlock()
{
    AliceCodeBlock* CBlock = __AlAllocate(sizeof(AliceCodeBlock));
    CBlock -> CloseFormat = NULL;
    CBlock -> OpenFormat = NULL;
    CBlock -> LineFormat = NULL;
    CBlock -> HasFormat = False;
    CBlock -> LineCount = 0;
    CBlock -> BlockCount = 0;
    CBlock -> CodeLines = __NewAlVector(sizeof(AliceCodeContext));
    return CBlock;
}

Bool NewCodeBlockLine(AliceCodeBlock* Block, AliceCodeLine* Line)
{
    __PushAlVector(Block -> CodeLines, NewCodeContext(AlContext_Line, Line));
    Block -> LineCount++;
}

Bool NewCodeBlockBlock(AliceCodeBlock* Block, AliceCodeBlock* OtherBlock)
{
    __PushAlVector(Block -> CodeLines, NewCodeContext(AlContext_Block, OtherBlock));
    Block -> BlockCount++;
}

Bool NewCodeWriterBlock(AliceCodeWritter* Writter, AliceCodeBlock* Block)
{
    __PushAlVector(Writter -> WriteContent, Block);
}

Bool NewAliceCodeWritterLine(AliceCodeWritter* Writter, AliceCodeLine* Line)
{
    AliceCodeBlock* TemporaryBlock = NewCodeBlock();
    NewCodeBlockLine(TemporaryBlock, Line);
    NewCodeWriterBlock(Writter, TemporaryBlock);
    Writter -> CurrentBlock = TemporaryBlock;
}

AliceCodeFormat* NewCodeFormat(CString Literal)
{
    AliceCodeFormat* CFormat = __AlAllocate(sizeof(AliceCodeFormat));
    CFormat -> Content = __AlStringFrom(Literal);
    return CFormat;
}

AliceCodeBlock* NewCodeBlockFormatted(CString OpenFormat, CString CloseFormat)
{
    AliceCodeBlock* Block = NewCodeBlock();
    Block -> OpenFormat = NewCodeFormat(OpenFormat);
    Block -> CloseFormat = NewCodeFormat(CloseFormat);
    return Block;
}

__AliceString* GetCodeByContext(AliceCodeWritter* Writter, AliceCodeContext* Context)
{
    __AliceString* Result = __AlStringFrom("");
    switch (Context -> Kind)
    {
        case AlContext_Line:
            __AppendAlString(Result, Context -> Content.Line -> Content -> Chars);
            break;
        case AlContext_Block:
            if (Context -> Content.Block -> OpenFormat != NULL) __AppendAlString(Result, Context -> Content.Block -> OpenFormat -> Content -> Chars);
            for (
                Usize Interator = 0;
                Interator < Context -> Content.Block -> CodeLines -> VSize;
                Interator += 1
            )
            {
                AliceCodeContext* Child = (AliceCodeContext*)__GetAlVector(Context -> Content.Block -> CodeLines, Interator);
                __AliceString* ChildResult = (__AliceString*)GetCodeByContext(Writter, Child);
                __AppendAlString(Result, ChildResult -> Chars);
            }
            if (Context -> Content.Block -> CloseFormat != NULL) __AppendAlString(Result, Context -> Content.Block -> CloseFormat -> Content -> Chars);
            break;
    }
    return Result;
}

Bool CodeWrite(AliceCodeWritter* Writter)
{
    __AliceString* StringWrite = CodeGenerateCodeWritter(Writter);
    AliceFileWrite(Writter -> TargetPath -> Chars, StringWrite -> Chars, "w");
}

__AliceString* CodeGenerateCodeWritter(AliceCodeWritter* Writter)
{
    __AliceString* StringWrite = __AlStringFrom("");
    for (
        Usize Interator = 0;
        Interator < Writter -> WriteContent -> VSize;
        Interator += 1
    )
    {
        __AppendAlString
        (
            StringWrite, 
            GetCodeByContext(Writter, 
                NewCodeContext(AlContext_Block, (AliceCodeBlock*)__GetAlVector(Writter -> WriteContent, Interator))
        ) -> Chars);
    }
    return StringWrite;
}

AliceCompilerResult* NewAliceCompilerResult(Bool Ok, __AliceString* Literal)
{
    AliceCompilerResult* CResult = __AlAllocate(sizeof(AliceCompilerResult));
    CResult -> Ok = Ok;
    CResult -> RLiteral = Literal;
    return CResult; 
}

Bool GetCompilationWasSucessfull(AliceCompilerResult* Result)
{
    return (Result != NULL) && (Result -> Ok) && (Result -> RLiteral != NULL) && (Result -> RLiteral -> SCapacity > 0);
}