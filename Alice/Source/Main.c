#include "External/Libraries.h"
#include "Headers/ArchiveManager.h"
#include "Headers/StringUtilities.h"
#include "Headers/InterpreterConstants.h"
#include "Headers/Lexer.h"
#include "Headers/Parser.h"
#include "Headers/Cleaner.h"
#include "Headers/Reader.h"

int main(int argc, char* argv[]) {
    CompAttributes* CompilationAttributes = Reader(argc, argv);
    Alice_BigGirl BigGirl;
    Vector Tokens;
    New(&Tokens, sizeof(Alice_Token));
    GirlInit(&BigGirl);
    ReadPath(CompilationAttributes, &BigGirl);
    Clean(&BigGirl);
    Tokenize(&BigGirl, &Tokens);
    Parsenize(&BigGirl, &Tokens);

    String* x = FromStr("");
    for (int i = 0 ; i < Len(&Tokens) ; i++) 
    {
        char p[50];
        Alice_Token* t = Get(&Tokens, i);
        String* s = FromStr("");
        char* c = Substring(BigGirl.SourceCode -> Chars, t -> Start, t-> Start + t -> Length);
        if (t -> Type == Tk_Dedent) {
            s = FromStr("<_DEDENT_>");
        } else if (t -> Type == Tk_Indent) {
            s = FromStr("<_INDENT_>");
        } else {
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
    return 0;
}
