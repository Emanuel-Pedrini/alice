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

Alice_BigGirl* GirlNew()
{
    Alice_BigGirl* Girl = malloc(sizeof(Alice_BigGirl));
    VECTOR(TokenVector, Alice_Token)

    Girl -> CodeObjects = TokenVector;
    Girl -> SourceCode = FromStr("");
    return Girl;
}