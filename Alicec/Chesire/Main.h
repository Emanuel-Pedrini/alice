#ifndef _MainH_
#define _MainH_

#include "External/Libraries.h"
#include "Headers/ForwardInclude.h"
#include "Headers/AliceDevelopmentTypes.h"

typedef struct Struct__ChesireCommandReader ChesireCommandReader;
typedef ChesireCommandReader* ChesireCommandReaderRef;

struct Struct__ChesireCommandReader
{
    long Pointer;
    long ArgumentCount;
    char** Arguments;
};

void InitializeChesireCommandReader(ChesireCommandReaderRef Reader, long ArgumentCount, char** Arguments);

int ChesireAcessFile(char* Path, const char* Content, const char* Extension, const char* Mode);
#endif