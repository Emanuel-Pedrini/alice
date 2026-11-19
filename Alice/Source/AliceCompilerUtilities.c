#include "Compiler/AliceCompilerUtilities.h"

CodeBlock* CBlock()
{
    CBLOCK(B)
    __AliceVector* V = __NewAlVector(sizeof(CodeLine));
    B -> HasFormat = False;
    B -> CodeLines = V;
    return B;
}

Bool CBlockAppend(CodeBlock* B, CodeLine* L)
{
    __PushAlVector(B -> CodeLines, L);
    return True;
}


CodeLine* CLine(const char* Content)
{
    CLINE(L)
    L -> Content = __AlStringFrom(Content);
    return L;
}

CodeWritter* CWritter(const char* TargetPath)
{
    CWRITTER(W)
    __AliceVector* V = __NewAlVector(sizeof(CodeLine));
    W -> TargetPath = __AlStringFrom(TargetPath);
    W -> Pointer = (Usize) 0;
    W -> WriteContent = V;
    return W;
}

Bool CWriteLine(CodeWritter* W, CodeLine* L)
{
    CLINE(LA)
    LA -> Content = __ConcatenateAlString(2, L -> Content, __AlStringFrom("\n"));
    FileWrite((W -> TargetPath) -> Chars, (LA -> Content) -> Chars, "a");
}
Bool CWriteBlock(CodeWritter* W, CodeBlock* B)
{
    for (int I = 0 ; I < B -> CodeLines -> VSize ; I++)
    {
        CWriteLine(W, __GetAlVector(B -> CodeLines, I));
    }
}

Bool CWrite(CodeWritter* W)
{
    __AliceString* S = __AlStringFrom("");
    for (int I = 0 ; I < W -> WriteContent -> VSize; I++)
    {
        CodeLine* L = (CodeLine*)__GetAlVector(W -> WriteContent, I);
        __AppendAlString(S, L -> Content -> Chars);
    }
    int R = FileWrite((W -> TargetPath) -> Chars, S -> Chars, "w");
    if (R) return False;
    return True;
}

Bool CWritterAppendLine(CodeWritter* W, CodeLine* L)
{
    CLINE(LA)
    LA -> Content = __AlStringFrom("\n");
    __AppendAlString(LA -> Content, L -> Content -> Chars);
    __PushAlVector(W -> WriteContent, LA);
    return True;
}

Bool CWriterAppendBlock(CodeWritter* W, CodeBlock* B)
{
    if (B -> HasFormat) CWritterAppendLine(W, CLine(B -> S -> Content -> Chars));   
    for (int I = 0 ; I < B -> CodeLines -> VSize ; I++)
    {
        CodeLine* L = ((CodeLine*)__GetAlVector(B -> CodeLines, I));
        CWritterAppendLine(W, L);
    }
    if (B -> HasFormat) CWritterAppendLine(W, CLine(B -> E -> Content -> Chars)); 
}

CodeFormat* CFormat(const char* Content)
{
    CFORMAT(F)
    F -> Content = __AlStringFrom(Content);
    return F;      
}

Bool CBlockFormat(CodeBlock* B, CodeFormat* S, CodeFormat* E)
{
    B -> HasFormat = True;
    B -> S = S;
    B -> E = E;
}

CodeBlock* CBlockFormatted(const char* Open, const char* Close)
{
    CodeBlock* B = CBlock();
    QFORMAT(F1, Open)
    QFORMAT(F2, Close)
    CBlockFormat(B, F1, F2);
    return B;
}