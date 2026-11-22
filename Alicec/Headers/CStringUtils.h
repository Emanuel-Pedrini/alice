#ifndef _CStringUtils_
#define _CStringUtils_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/ForwardInclude.h"

Bool IsNumerical(const char Character);
Bool IsInvalid(const char Character);
Bool IsAlphabetic(const char Character);
Bool IsAlphaNumerical(const char Character);
Bool IsIndentifierChar(const char Character);
Bool IsValidNumerical(const char Character);
char* Substring(CString Source, Usize Start, Usize End);
char* File(char* FullPath);
Bool CStringStartsWith(CString Source, CString Prefix);
Bool CStringEndsWith(CString Source, CString Suffix);
Bool IsEqual(CString Word, CString Source);
Bool IsCaseEqual(CString Word, CString Source);
#endif