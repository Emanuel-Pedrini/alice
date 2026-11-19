#ifndef _CodeObjectH_
#define _CodeObjectH_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"

typedef struct {
    char* Code;
    char* Path;
    char* Name;
} AliceCodeObject;

typedef struct {
    __AliceVector* CodeObjects;
    __AliceString* SourceCode;
    __AliceString* OutputPath;
} AliceManagementObject;

void GirlAppend(AliceManagementObject* Girl, AliceCodeObject* CodeObject);
AliceManagementObject* NewAliceManagementObject();

#endif