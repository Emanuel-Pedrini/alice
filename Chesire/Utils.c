#include <string.h>
#include "Headers/Utils.h"

Bool IsEqual(const char* Word, const char* Src) {
    if (strcmp(Word, Src) == 0) 
    {
        return fTrue;
    }
    return fFalse;
}