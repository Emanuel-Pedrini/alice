#ifndef _CodeObjectH_
#define _CodeObjectH_

#include "Headers/ArrayUtilities.h"
#include "Headers/StringUtilities.h"

typedef struct {
    char* Code;
    char* Path;
    char* Name;
} AliceCodeObject;

typedef struct {
    Vector* CodeObjects;
    String* SourceCode;
    String* OutputPath;
} AliceManagementObject;

void GirlAppend(AliceManagementObject* Girl, AliceCodeObject* CodeObject);
AliceManagementObject* NewAliceManagementObject();

#endif