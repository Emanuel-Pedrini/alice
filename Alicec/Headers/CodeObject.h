#ifndef _CodeObjectH_
#define _CodeObjectH_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/ForwardInclude.h"

typedef struct {
    char* Code;
    char* Path;
    char* Name;
} AliceCodeObject;

struct Struct__AliceManagementObject {
    __AliceVector* CodeObjects;
    __AliceString* SourceCode;
};

void IncludeManagementObject(AliceManagementObject* Girl, AliceCodeObject* CodeObject);
AliceManagementObject* NewAliceManagementObject();

#endif