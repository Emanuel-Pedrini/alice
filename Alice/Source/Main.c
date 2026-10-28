#include "External/Libraries.h"
#include "Headers/ArchiveManager.h"
#include "Headers/StringUtilities.h"
#include "Headers/InterpreterConstants.h"
#include "Headers/Lexer.h"
#include "Headers/Parser.h"
#include "Headers/Reader.h"
#include "Headers/DictionaryUtilities.h"
#include "Compiler/AliceCompilerUtilities.h"
#include "Compiler/AliceC.h"
#include "Headers/AliceTypes.h"

Bool ReadAndDecode(int argc, char* argv[])
{
    CompAttributes* CompilationAttributes = Reader(argc, argv);
    Alice_BigGirl* BigGirl = GirlNew();
    Bool Result = ReadPath(CompilationAttributes, BigGirl); 
    if (!Result) return 0;
    Vector* Tokens = Tokenize(BigGirl);
    Parsenize(BigGirl, Tokens);
    
    {

    /* 
    **TEMPORARY : ONLY FOR TESTS, REMOVE LATER. 
    */
    String* x = FromStr("");
    for (int i = 0 ; i < Len(Tokens) ; i++) 
    {
        char p[50];
        Alice_Token* t = (Alice_Token*)Get(Tokens, i);
        String* s = FromStr("");
        char* c = Substring(BigGirl -> SourceCode -> Chars, t -> Start, t-> Start + t -> Length);
        if (t -> Type == Tk_Dedent) 
        {
            s = FromStr("<_DEDENT_>");
        } 
        else if (t -> Type == Tk_Indent) 
        {
            s = FromStr("<_INDENT_>");
        } 
        else if (t -> Type == Tk_NewLine) 
        {
            s = FromStr("\\n");
        }
        else 
        {
            s = FromStr(c);
        }
        snprintf(p, sizeof(p), "(%d) ", t -> Type);
        AppendStr(x, p);
        AddStr(x, '\"');
        AppendStr(x, s -> Chars);
        AddStr(x, '\"');
        AddStr(x, '\n');
    }
        String* s = FromStr(CompilationAttributes -> TargetArchive);   
        AppendStr(s, ".txt");
        FileWrite(s -> Chars, x -> Chars, "w");
        printf("%d", CompilationAttributes -> Target);
    }
    return True;
}

int VirtualMain(int argc, char* argv[])
{
    ReadAndDecode(argc, argv);
    CodeWritter* W = CWritter("C:\\Users\\Emanuel\\Documents\\GitHub\\alice\\TestCompiler.c");
    CWriterAppendBlock(W, CLang_Include("stdio.h"));
    CodeBlock* MB = CLang_Main(CBlock());
    
    AliceType* T = malloc(sizeof(AliceType));
    T -> Kind = f64;
    CodeLine* VI = CLang_InitVariable("_X", T);
    CBlockAppend(MB, VI);
    
    CWriterAppendBlock(W, MB);
    CWrite(W);
    __AliceString* AlStr0 = __NewAlString();
    __AliceString* AlStr1 = __AlStringFrom("This is a string!");
    __AliceString* AlStr2 = __AlStringFrom("This is a string!");
    __AliceString* AlStr3 = __AlStringFrom("This is a string!");
    __AliceString* AlStr4 = __AlStringFrom("This is a string!");
    __AliceString* AlStr5 = __ConcatenateAlString(4, AlStr1, AlStr2, AlStr3, AlStr4);
     __AliceString* AlStr6 = __JoinAlString(" ", 4, AlStr1, AlStr2, AlStr3, AlStr4);
    printf("\n%s", AlStr6 -> Chars);
    return 0; 
}

int main(int argc, char* argv[]) 
{
    VirtualMain(argc, argv);
}

