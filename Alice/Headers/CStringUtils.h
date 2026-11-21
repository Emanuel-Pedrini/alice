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
char* Substring(__CString Source, Usize Start, Usize End);
char* File(char* FullPath);
Bool StartsWith(__CString Source, __CString Prefix);
Bool EndsWith(__CString Source, __CString Suffix);
Bool IsEqual(__CString Word, __CString Source);
Bool IsCaseEqual(__CString Word, __CString Source);
#endif