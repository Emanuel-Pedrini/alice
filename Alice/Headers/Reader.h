#ifndef _ReaderH_
#define _ReaderH_

typedef enum {
    C = 0,
    LLVM = 1 
} TargetAttribute;

typedef enum {
    COMPILE_INDIVIDUALLY = 1
} Specifications;

typedef struct StructCompAttributes {
    TargetAttribute Target;
    String* Paths;
    Vector* Specifications;
    char* TargetArchive;
} CompAttributes;

CompAttributes* Reader(int NumberOfArguments, char** Arguments);
void ReadCallArgument(CompAttributes* Attributes, char* Argument);
Bool ReadPath(CompAttributes* CompilationAttributes, Alice_BigGirl* BigGirl);

#endif