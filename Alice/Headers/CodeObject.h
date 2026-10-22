#ifndef _CodeObjectH_
#define _CodeObjectH_

#include "Headers/ArrayUtilities.h"
#include "Headers/StringUtilities.h"

typedef struct {
    char* Code;
    char* Path;
    char* Name;
} Alice_CodeObject;

typedef struct {
    Vector* CodeObjects;
    String* SourceCode;
    String* OutputPath;
} Alice_BigGirl;

void GirlAppend(Alice_BigGirl* Girl, Alice_CodeObject* CodeObject);
void GirlInit(Alice_BigGirl* Girl);

#endif