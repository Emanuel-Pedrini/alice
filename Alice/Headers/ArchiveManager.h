#ifndef _ArchiveManagerH_
#define _ArchiveManagerH_

#include "Headers/AliceMemoryManagement.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/ForwardInclude.h"
#include "Headers/CodeObject.h"

char* OpenFile(char* ArchivePath, Usize Capacity);
AliceCodeObject* OpenAliceFile(char* ArchivePath);
int FileWrite(char* Path, __CString Content, __CString Mode);
int BinaryWrite(char* Path, unsigned int Content);

#endif