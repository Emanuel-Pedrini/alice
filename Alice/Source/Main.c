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
#include "Headers/PrattParsing.h"
#include "Compiler/Native/AliceLLVM.h"

Bool ReadAndDecode(int argc, char* argv[])
{
    CompAttributes* CompilationAttributes = ReadCompilationAttributes(argc, argv);
    AliceManagementObject* BigGirl = NewAliceManagementObject();
    Bool Result = ReadCompilationPaths(CompilationAttributes, BigGirl); 
    if (!Result) return 0;
    Vector* Tokens = AliceLexerTokenize(BigGirl -> SourceCode);
    Parsenize(BigGirl, Tokens);
    
    {

    /* 
    **TEMPORARY : ONLY FOR TESTS, REMOVE LATER. 
    */
    String* x = FromStr("");
    for (int i = 0 ; i < Len(Tokens) ; i++) 
    {
        char p[50];
        AliceToken* t = (AliceToken*)GetByIndexVector(Tokens, i);
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
    }
    return True;
}

int VirtualMain(int argc, char* argv[])
{
    ReadAndDecode(argc, argv);
    return 0;
}

int main(int argc, char* argv[]) 
{
    return VirtualMain(argc, argv);
}

