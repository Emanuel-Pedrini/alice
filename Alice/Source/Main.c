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

    CodeWritter* __CLangWritter = CWritter("C:\\Users\\Emanuel\\Documents\\GitHub\\alice\\CompilerExamples\\TestCompiler.c");
    CWriterAppendBlock(__CLangWritter, CLang_Include("stdio.h"));
    CWriterAppendBlock(__CLangWritter, CLang_Include("stdint.h"));
    CWriterAppendBlock(__CLangWritter, CLang_Include("AliceTypes.h"));
    CodeBlock* __MainBlock = CLang_Main();
    
    CodeLine* __V1 = CLang_InitVariable("__Float32", &__AliceTypePtr_f32, __MainBlock);
    CodeLine* __V2 = CLang_InitVariable("__Integer8", &__AliceTypePtr_i8, __MainBlock);

    CWriterAppendBlock(__CLangWritter, __MainBlock);
    CWrite(__CLangWritter);
    return 0;
}

int main(int argc, char* argv[]) 
{
    return VirtualMain(argc, argv);
}

