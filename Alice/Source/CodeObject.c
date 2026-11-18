#include "Headers/CodeObject.h"
#include "External/Libraries.h"
#include "Headers/Lexer.h"
void GirlAppend(AliceManagementObject* Girl, AliceCodeObject* CodeObject) 
{
    Add(Girl -> CodeObjects, CodeObject);

    AddStr(Girl -> SourceCode, ARCHIVE_MARKER);
    for (int x = 0; CodeObject -> Name[x] != '\0'; x++) 
    {
        AddStr(Girl -> SourceCode, CodeObject -> Name[x]);
    }
    AddStr(Girl -> SourceCode, ARCHIVE_MARKER);
    AddStr(Girl -> SourceCode, ' ');

    for (int x = 0; CodeObject -> Code[x] != '\0'; x++) 
    {
        AddStr(Girl -> SourceCode, CodeObject -> Code[x]);
    }
}

AliceManagementObject* NewAliceManagementObject()
{
    AliceManagementObject* Girl = malloc(sizeof(AliceManagementObject));
    VECTOR(TokenVector, AliceToken)

    Girl -> CodeObjects = TokenVector;
    Girl -> SourceCode = FromStr("");
    return Girl;
}