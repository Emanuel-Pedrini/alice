#include "Headers/CStringUtils.h"

Bool IsNumerical(const char Character) 
{
    if (Character >= '0' && Character <= '9') {
        return True;
    }
    return False;
}

Bool IsValidNumerical(const char Character) {
    if (Character >= '0' && Character <= '9') {
        return True;
    }
    return False;
}

Bool IsInvalid(const char Character) 
{
    if (Character != '\0') {
        return False;
    }
    return True;
}

Bool IsAlphabetic(const char Character)
{
    if ((Character >= 'a' && Character <= 'z') || (Character >= 'A' && Character <= 'Z')) {
        return True;
    }
    return False;
}

Bool IsAlphaNumerical(const char Character)
{
    if (IsAlphabetic(Character) || IsNumerical(Character))
    {
        return True;
    }
    return False;
}

Bool IsIndentifierChar(const char Character)
{
    if (IsAlphabetic(Character) || IsNumerical(Character) || Character == '_')
    {
        return True;
    }
    return False;
}

char* File(char* FullPath) {
    Usize StringSize = strlen(FullPath);
    int x;
    for (x = StringSize; FullPath[x] != '\\' &&  FullPath[x] != '/'; x--) 
    {
    }
    return Substring(FullPath, x + 1, StringSize);
}

Bool StartsWith(__CString Source, __CString Prefix)
{
    Usize PreffixSize = strlen(Prefix);
    Usize stringSize = strlen(Source);
    if (!(PreffixSize > stringSize)) 
    {
        char* StringStart = Substring(Source, 0, PreffixSize);
        if (strcmp(StringStart, Prefix) == 0) 
        {
            return True;
        }
        return False;
    }
    return False;
}

Bool EndsWith(__CString Source, __CString Suffix)
{
    Usize PreffixSize = strlen(Suffix);
    Usize StringSize = strlen(Source);
    if (!(PreffixSize > StringSize)) {
        char* stringStart = Substring(Source, StringSize - PreffixSize, StringSize);
        if (strcmp(stringStart, Suffix) == 0) {
            return True;
        }
        return False;
    }
    return False;
}
Bool IsEqual(__CString Word, __CString Source) {
    if (strcmp(Word, Source) == 0) 
    {
        return True;
    }
    return False;
}

Bool IsCaseEqual(__CString Word, __CString Source) {
    if (strcasecmp(Word, Source) == 0) 
    {
        return True;
    }
    return False;
}

char* Substring(__CString Source, Usize Start, Usize End)
{
    int PretendedSize = (End - Start) + 1;
    if (PretendedSize <= 0) {
        return NULL;
    }
    Usize SubstringSize = PretendedSize;
    char* TemporaryBuffer = __AlAllocate(SubstringSize * sizeof(unsigned char));
    if (TemporaryBuffer != NULL) {
        Usize x = Start;
        Usize y = 0;
        for (; x < End && Source[x] != '\0' ; x++, y++) {
            TemporaryBuffer[y] = Source[x];
        }
        TemporaryBuffer[y] = '\0';
        return TemporaryBuffer;
    }
    return NULL;
}