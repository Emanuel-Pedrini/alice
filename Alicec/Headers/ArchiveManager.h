#ifndef _ArchiveManagerH_
#define _ArchiveManagerH_

#include "Headers/AliceMemoryManagement.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/ForwardInclude.h"
#include "Headers/CodeObject.h"

char* OpenFileNWindows(char* ArchivePath, Usize Capacity);
AliceCodeObject* OpenAliceFile(char* ArchivePath);
int FileWrite(char* Path, CString Content, CString Mode);
int BinaryWrite(char* Path, unsigned int Content);
int AlicePathExists(CString Path);
int ListDirectory(CString Folder, CString Filter);
#endif