#include "Compiler/AliceCompilerUtilities.h"

__AliceCodeBlock* CBlock()
{
    CBLOCK(B)
    __AliceVector* V = __NewAlVector(sizeof(__AliceCodeLine));
    B -> HasFormat = False;
    B -> CodeLines = V;
    return B;
}

Bool CBlockAppend(__AliceCodeBlock* B, __AliceCodeLine* L)
{
    __PushAlVector(B -> CodeLines, L);
    return True;
}


__AliceCodeLine* CLine(__CString Content)
{
    CLINE(L)
    L -> Content = __AlStringFrom(Content);
    return L;
}

CodeWritter* CWritter(__CString TargetPath)
{
    CWRITTER(W)
    __AliceVector* V = __NewAlVector(sizeof(__AliceCodeLine));
    W -> TargetPath = __AlStringFrom(TargetPath);
    W -> Pointer = (Usize) 0;
    W -> WriteContent = V;
    return W;
}

Bool CWriteLine(CodeWritter* W, __AliceCodeLine* L)
{
    CLINE(LA)
    LA -> Content = __ConcatenateAlString(2, L -> Content, __AlStringFrom("\n"));
    FileWrite((W -> TargetPath) -> Chars, (LA -> Content) -> Chars, "a");
}
Bool CWriteBlock(CodeWritter* W, __AliceCodeBlock* B)
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
        __AliceCodeLine* L = (__AliceCodeLine*)__GetAlVector(W -> WriteContent, I);
        __AppendAlString(S, L -> Content -> Chars);
    }
    int R = FileWrite((W -> TargetPath) -> Chars, S -> Chars, "w");
    if (R) return False;
    return True;
}

Bool CWritterAppendLine(CodeWritter* W, __AliceCodeLine* L)
{
    CLINE(LA)
    LA -> Content = __AlStringFrom("\n");
    __AppendAlString(LA -> Content, L -> Content -> Chars);
    __PushAlVector(W -> WriteContent, LA);
    return True;
}

Bool CWriterAppendBlock(CodeWritter* W, __AliceCodeBlock* B)
{
    if (B -> HasFormat) CWritterAppendLine(W, CLine(B -> S -> Content -> Chars));   
    for (int I = 0 ; I < B -> CodeLines -> VSize ; I++)
    {
        __AliceCodeLine* L = ((__AliceCodeLine*)__GetAlVector(B -> CodeLines, I));
        CWritterAppendLine(W, L);
    }
    if (B -> HasFormat) CWritterAppendLine(W, CLine(B -> E -> Content -> Chars)); 
}

__AliceCodeFormat* CFormat(__CString Content)
{
    CFORMAT(F)
    F -> Content = __AlStringFrom(Content);
    return F;      
}

Bool CBlockFormat(__AliceCodeBlock* B, __AliceCodeFormat* S, __AliceCodeFormat* E)
{
    B -> HasFormat = True;
    B -> S = S;
    B -> E = E;
}

__AliceCodeBlock* CBlockFormatted(__CString Open, __CString Close)
{
    __AliceCodeBlock* B = CBlock();
    QFORMAT(F1, Open)
    QFORMAT(F2, Close)
    CBlockFormat(B, F1, F2);
    return B;
}