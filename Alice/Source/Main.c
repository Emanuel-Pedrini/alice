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
    ExpressionReader* R = NewExpressionReader();
    Alice_Token* TP1 = CreateToken(Tk_LeftParenthesis, 0, 0, "(");
    Alice_Token* T1 = CreateToken(Tk_Number, 0, 0, "1");
    Alice_Token* T2 = CreateToken(Tk_Plus, 0, 0, "^");
    Alice_Token* T3 = CreateToken(Tk_Number, 0, 0, "2");
    Alice_Token* TP2 = CreateToken(Tk_RightParenthesis, 0, 0, ")");
    Alice_Token* T4 = CreateToken(Tk_Asterisk, 0, 0, "^");
    Alice_Token* T5 = CreateToken(Tk_Number, 0, 0, "3");
    VECTOR(V, Alice_Token)
    Add(V, T1);
    Add(V, T2);
    Add(V, T3);
    Add(V, T4);
    Add(V, T5);
    R -> ConstExpression = V;

    Expression* Exp = ParseExpression(R, 0);
    printf("\n%s", Exp -> ConversionToString(Exp) -> Chars);
    return 0;
}

int main(int argc, char* argv[]) 
{
    return VirtualMain(argc, argv);
}

