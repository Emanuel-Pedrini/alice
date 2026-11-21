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
char* Substring(const char* Source, Usize Start, Usize End);
char* File(char* FullPath);
Bool StartsWith(const char* Source, const char* Prefix);
Bool EndsWith(const char* Source, const char* Suffix);
Bool IsEqual(const char* Word, const char* Source);
Bool IsCaseEqual(const char* Word, const char* Source);
#endif