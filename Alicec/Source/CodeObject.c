#include "Headers/CodeObject.h"
#include "External/Libraries.h"
#include "Headers/Lexer.h"
#include "Headers/ForwardInclude.h"

void IncludeManagementObject(AliceManagementObject* Girl, AliceCodeObject* CodeObject) 
{
    __PushAlVector(Girl -> CodeObjects, CodeObject);

    __AppendAlString(Girl -> SourceCode, ARCHIVE_MARKER);
    __AppendAlString(Girl -> SourceCode, CodeObject -> Name);
    __AppendAlString(Girl -> SourceCode, ARCHIVE_MARKER);
    __AppendAlString(Girl -> SourceCode, " ");
    __AppendAlString(Girl -> SourceCode, CodeObject -> Code);
}

AliceManagementObject* NewAliceManagementObject()
{
    AliceManagementObject* Girl = __AlAllocate(sizeof(AliceManagementObject));
    Girl -> CodeObjects = __NewAlVector(sizeof(AliceCodeObject));
    Girl -> SourceCode = __AlStringFrom("");
    return Girl;
}