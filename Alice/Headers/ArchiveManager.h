#ifndef _ArchiveManagerH_
#define _ArchiveManagerH_

#include "Headers/ArrayUtilities.h"
#include "Headers/CodeObject.h"

char* OpenFile(char* ArchivePath, Usize Capacity);
Alice_CodeObject* OpenAliceFile(char* ArchivePath);
int FileWrite(char* Path, const char* Content, const char* Mode);
int BinaryWrite(char* Path, unsigned int Content);

#endif