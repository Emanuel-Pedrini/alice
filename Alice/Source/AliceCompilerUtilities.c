#include "Compiler/AliceCompilerUtilities.h"

CodeBlock* CBlock()
{
    CBLOCK(B)
    VECTOR(V, CodeLine)
    B -> HasFormat = False;
    B -> CodeLines = V;
    return B;
}

Bool CBlockAppend(CodeBlock* B, CodeLine* L)
{
    Add(B -> CodeLines, L);
    return True;
}


CodeLine* CLine(const char* Content)
{
    CLINE(L)
    L -> Content = PSTR(Content);
    return L;
}

CodeWritter* CWritter(const char* TargetPath)
{
    CWRITTER(W)
    W -> TargetPath = PSTR(TargetPath);
    W -> Pointer = (Usize) 0;
    VECTOR(V, CodeLine)
    W -> WriteContent = V;
    return W;
}

Bool CWriteLine(CodeWritter* W, CodeLine* L)
{
    CLINE(LA)
    LA -> Content = ConcatStr(L -> Content, PSTR("\n"));
    FileWrite(SPTR(W -> TargetPath), SPTR(LA -> Content), "a");
}
Bool CWriteBlock(CodeWritter* W, CodeBlock* B)
{
    for (int I = 0 ; I < B -> CodeLines -> size ; I++)
    {
        CWriteLine(W, Get(B -> CodeLines, I));
    }
}

Bool CWrite(CodeWritter* W)
{
    String* S = FromStr("");
    for (int I = 0 ; I < W -> WriteContent -> size; I++)
    {
        CodeLine* L = (CodeLine*)Get(W -> WriteContent, I);
        AppendStr(S, L -> Content -> Chars);
    }
    int R = FileWrite(SPTR(W -> TargetPath), S -> Chars, "w");
    if (R) return False;
    return True;
}

Bool CWritterAppendLine(CodeWritter* W, CodeLine* L)
{
    CLINE(LA)
    LA -> Content = FromStr("\n");
    AppendStr(LA -> Content, L -> Content -> Chars);
    Add(W -> WriteContent, LA);
    return True;
}

Bool CWriterAppendBlock(CodeWritter* W, CodeBlock* B)
{
    if (B -> HasFormat) CWritterAppendLine(W, CLine(B -> S -> Content -> Chars));   
    for (int I = 0 ; I < B -> CodeLines -> size ; I++)
    {
        CodeLine* L = ((CodeLine*)Get(B -> CodeLines, I));
        CWritterAppendLine(W, L);
    }
    if (B -> HasFormat) CWritterAppendLine(W, CLine(B -> E -> Content -> Chars)); 
}

CodeFormat* CFormat(const char* Content)
{
    CFORMAT(F)
    F -> Content = PSTR(Content);
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