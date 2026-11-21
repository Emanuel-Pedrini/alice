#ifndef _ReaderH_
#define _ReaderH_

#include "Headers/AliceMemoryManagement.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/ForwardInclude.h"

typedef enum {
    C = 0,
    LLVM = 1 
} TargetAttribute;

typedef enum {
    COMPILE_INDIVIDUALLY = 1
} Specifications;

typedef struct StructCompAttributes {
    TargetAttribute Target;
    __AliceString* Paths;
    __AliceVector* Specifications;
    char* TargetArchive;
} CompAttributes;

CompAttributes* ReadCompilationAttributes(int NumberOfArguments, char** Arguments);
void ReadCallArgument(CompAttributes* Attributes, char* Argument);
Bool ReadCompilationPaths(CompAttributes* CompilationAttributes, AliceManagementObject* BigGirl);

#endif