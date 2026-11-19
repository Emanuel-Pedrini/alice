#include "External/Libraries.h"
#include "Headers/ArchiveManager.h"
#include "Headers/InterpreterConstants.h"
#include "Headers/Lexer.h"
#include "Headers/Parser.h"
#include "Headers/Reader.h"
#include "Compiler/AliceCompilerUtilities.h"
#include "Compiler/AliceC.h"
#include "Headers/AliceTypes.h"
#include "Headers/PrattParsing.h"
#include "Compiler/Native/AliceLLVM.h"

Bool ReadAndDecode(int argc, char* argv[])
{
    CompAttributes* CompilationAttributes = ReadCompilationAttributes(argc, argv);
    AliceManagementObject* BigGirl = NewAliceManagementObject();
    Bool Result = ReadCompilationPaths(CompilationAttributes, BigGirl); 
    if (!Result) return 0;
    __AliceVector* Tokens = AliceLexerTokenize(BigGirl -> SourceCode);
    ParserState* AliceParserState = NewParserState(Tokens);
    __AliceVector* Nodes = Parsenize(AliceParserState, BigGirl);
    {

    /* 
    **TEMPORARY : ONLY FOR TESTS, REMOVE LATER. 
    */
    __AliceString* x = __AlStringFrom("");
    for (int i = 0 ; i < Tokens -> VSize ; i++) 
    {
        char p[50];
        AliceToken* t = (AliceToken*)__GetAlVector(Tokens, i);
        __AliceString* s = __AlStringFrom("");
        char* c = t -> Literal;
        if (t -> Type == Tk_Dedent) 
        {
            s = __AlStringFrom("<_DEDENT_>");
        } 
        else if (t -> Type == Tk_Indent) 
        {
            s = __AlStringFrom("<_INDENT_>");
        } 
        else if (t -> Type == Tk_NewLine) 
        {
            s = __AlStringFrom("\\n");
        }
        else 
        {
            s = __AlStringFrom(c);
        }
        snprintf(p, sizeof(p), "(%d) ", t -> Type);
        __AppendAlString(x, p);
        __AppendAlString(x, "\"");
        __AppendAlString(x, s -> Chars);
        __AppendAlString(x, "\"");
        __AppendAlString(x, "\n");
    }
        __AliceString* s = __AlStringFrom(CompilationAttributes -> TargetArchive);   
        __AppendAlString(s, ".txt");
        FileWrite(s -> Chars, x -> Chars, "w");
    }
    return True;
}

int VirtualMain(int argc, char* argv[])
{
    ReadAndDecode(argc, argv);
    __AlFreeAll();
    return 0;
}

int Debug__PrattParsenizeExpression()
{
    char Input[128];
    __AliceString* S = __AlStringFrom("");
    char ActualInput = '\0';
    while (ActualInput != '\n')
    {
        ActualInput = getchar();
        __AddCharAlString(S, ActualInput);
    }
    printf("\n%s", S -> Chars);
    __AliceVector* Tokens = AliceLexerTokenize(S);
    __AliceVector* CleanTokens = __NewAlVector(sizeof(AliceToken));
    __ForIn(Tokens, I, 0) 
    {
        AliceToken* T = __GetAlVector(Tokens, I);
        if (!(T -> Type == Tk_EndOfCode || T -> Type == Tk_ArchiveMarker))
        {
            __PushAlVector(CleanTokens, T);
        }
    }
    ExpressionReader* R = NewExpressionReader();
    R -> ConstExpression = CleanTokens;
    R -> Pointer = 0;
    Expression* E = ParseExpression(R, 0);
    printf("\n%s", E -> ConversionToString(E) -> Chars);
    return 0;
}

int main(int argc, char* argv[]) 
{
    return VirtualMain(argc, argv);
}

