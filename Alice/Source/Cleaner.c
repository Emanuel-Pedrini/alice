#include "Headers/Cleaner.h"
#include "Headers/Lexer.h"
#include "Headers/StringUtilities.h"
#include "External/Libraries.h"

char SemicolonToNewLine(char Char) 
{
    if (Char == ';') 
    {
        return '\n';
    }
    return Char;
}

Bool IsPriorityDeclr(char* Line) 
{
    if (StartsWith(Line, CONSTANT_KEYWORD)) 
    {
        return True;
    }
    return False;
}
char* ReplaceAliases(Alice_BigGirl* BigGirl)
{
    for (int x = 0; x < BigGirl -> SourceCode -> Size ; x++) {
        BigGirl -> SourceCode -> Chars[x] = SemicolonToNewLine(BigGirl -> SourceCode -> Chars[x]);
    }
}
char* GetLine(Alice_BigGirl* BigGirl, Usize Start) 
{
    int Length = 0;
    for (int x = Start ; BigGirl -> SourceCode -> Chars[x] != '\n' 
        && BigGirl -> SourceCode -> Chars[x] != '\0'; x++)
    {
        Length++;
    }
    return Substring(BigGirl -> SourceCode -> Chars, Start, Start + Length);
}

void Organize(Alice_BigGirl* BigGirl) 
{
    String* ConstantDeclarations = FromStr("");
    String* RestingCode = FromStr("");
    for (int x = 0 ; x < BigGirl -> SourceCode -> Size ;)
    {
        char* Line = GetLine(BigGirl, x);
        if (IsPriorityDeclr(Line)) {
            AppendStr(ConstantDeclarations, Line);
            AddStr(ConstantDeclarations, '\n');
        }
        else {
            AppendStr(RestingCode, Line);
            AddStr(RestingCode, '\n');
        }
        x += strlen(Line) + 1;
    }
    String* FinalCode = ConcatStr(ConstantDeclarations, RestingCode);

    // for (int x = 0;x < FinalCode -> Size ;x++) {
    //     printf("%c", FinalCode -> Chars[x]);
    // }
    AppendStr(FinalCode, END_OF_CODE_KEYWORD);
    BigGirl -> SourceCode = FinalCode;
}

void Clean(Alice_BigGirl* BigGirl) 
{
    Vector Lines;
    New(&Lines, sizeof(String));
    ReplaceAliases(BigGirl);
    Organize(BigGirl);
}

