#include "Headers/CodeObject.h"
#include "External/Libraries.h"
#include "Headers/Lexer.h"
void GirlAppend(Alice_BigGirl* Girl, Alice_CodeObject* CodeObject) 
{
    Add(Girl -> CodeObjects, CodeObject);
    
    AddStr(Girl -> SourceCode, '\n');
    AddStr(Girl -> SourceCode, ARCHIVE_MARKER);
    for (int x = 0; CodeObject -> Name[x] != '\0'; x++) 
    {
        AddStr(Girl -> SourceCode, CodeObject -> Name[x]);
    }
    AddStr(Girl -> SourceCode, ARCHIVE_MARKER);
    AddStr(Girl -> SourceCode, '\n');

    for (int x = 0; CodeObject -> Code[x] != '\0'; x++) 
    {
        AddStr(Girl -> SourceCode, CodeObject -> Code[x]);
    }
    AddStr(Girl -> SourceCode, '\n');
}

void GirlInit(Alice_BigGirl* Girl)
{
    Vector* TokenVector = malloc(sizeof(Vector));
    New(TokenVector, sizeof(Alice_CodeObject));

    Girl -> CodeObjects = TokenVector;
    Girl -> SourceCode = FromStr("");
}