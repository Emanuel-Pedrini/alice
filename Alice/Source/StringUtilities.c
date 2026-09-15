#include "Headers/StringUtilities.h"
#include "Headers/InterpreterTypes.h"
#include "Headers/ArrayUtilities.h"
#include "External/Libraries.h"

char* Substring(char* String, Usize Start, Usize End)
{
    int pretendedSize = (End - Start) + 1;
    if (pretendedSize <= 0) {
        return NULL;
    }
    Usize substringSize = pretendedSize;
    char* temporaryBuffer = malloc(substringSize * sizeof(unsigned char));
    if (temporaryBuffer != NULL) {
        Usize x = Start, y = 0;
        for (; x < End && String[x] != '\0' ; x++, y++) {
            temporaryBuffer[y] = String[x];
        }
        temporaryBuffer[y] = '\0';
        return temporaryBuffer;
    }
    return NULL;
}

Bool Is(const char* Word, const char* Src) {
    if (strcmp(Word, Src) == 0) 
    {
        return True;
    }
    return False;
}


Bool EndsWith(char* String, char* Suffix)
{
    Usize preffixSize = strlen(Suffix);
    Usize stringSize = strlen(String);
    if (!(preffixSize > stringSize)) {
        char* stringStart = Substring(String, stringSize - preffixSize, stringSize);
        if (strcmp(stringStart, Suffix) == 0) {
            return True;
        }
        return False;
    }
    return False;
}

Bool StartsWith(char* String, char* Prefix)
{
    Usize preffixSize = strlen(Prefix);
    Usize stringSize = strlen(String);
    if (!(preffixSize > stringSize)) {
        char* stringStart = Substring(String, 0, preffixSize);
        if (strcmp(stringStart, Prefix) == 0) {
            return True;
        }
        return False;
    }
    return False;
}

char* File(char* FullPath) {
    Usize stringSize = strlen(FullPath);
    int x;
    for (x = stringSize; FullPath[x] != '\\' &&  FullPath[x] != '/'; x--) {
    }
    return Substring(FullPath, x + 1, stringSize);
}

Bool IsNumerical(const char Char) 
{
    if (Char >= '0' && Char <= '9') {
        return True;
    }
    return False;
}

Bool IsValidNumerical(const char Char) {
    if ((Char >= '0' && Char <= '9') || (Char >= 'a' && Char <= 'z') || (Char >= 'A' && Char <= 'z')) {
        return True;
    }
    return False;
}

Bool IsInvalid(const char Char) 
{
    if (Char != '\0') {
        return False;
    }
    return True;
}

Bool IsAlphabetic(const char Char)
{
    if ((Char >= 'a' && Char <= 'z') || (Char >= 'A' && Char <= 'Z')) {
        return True;
    }
    return False;
}

Bool IsAlphaNumerical(const char Char)
{
    if (IsAlphabetic(Char) || IsNumerical(Char))
    {
        return True;
    }
    return False;
}

Bool IsIndentifierChar(const char Char)
{
    if (IsAlphabetic(Char) || IsNumerical(Char) || Char == '_')
    {
        return True;
    }
    return False;
}

String* FromStr(const char* Src) {

    String* Buffer = malloc(sizeof(String));
    if (Buffer == NULL) {
        free(Buffer);
        return NULL;
    }
    Buffer -> Size = strlen(Src);
    Buffer -> Capacity = Buffer -> Size * 2 + 1;
    Buffer -> Chars = malloc(Buffer -> Capacity * sizeof(char));
    if (Buffer -> Chars == NULL) {
        free(Buffer -> Chars);
        return NULL;
    }
    memcpy(
        Buffer -> Chars,
        Src,
        Buffer -> Size  + 1
    );
    return Buffer;
}

void NewStr(String* Str)
{

}

void AddStr(String* Str, const char Char)
{
    if (Str -> Size + 1 >= Str -> Capacity) {
        Usize NewCapacity = Str -> Capacity * 2 + 1;
        char* NewChars = realloc(Str -> Chars, NewCapacity * sizeof(char));
        if (NewChars == NULL) {
            free(NewChars);
            return;
        }
        Str -> Chars = NewChars;
        Str -> Capacity = NewCapacity;
    }
    Str -> Chars[Str -> Size] = Char;
    Str -> Size++;
    Str -> Chars[Str -> Size] = '\0';
}

void AppendStr(String* Str, char* Src)
{
    int Size = strlen(Src);
    for (int x = 0 ; x < Size ; x++) 
    {
        AddStr(Str, Src[x]);
    }
}

void InsertStr(String* Str, const char Char, Usize Position)
{
    if (Str -> Size + 1 >= Str -> Capacity) {
        Usize NewCapacity = Str -> Capacity * 2 + 1;
        char* NewChars = realloc(Str -> Chars, NewCapacity * sizeof(char));
        if (NewChars == NULL) {
            free(NewChars);
            return;
        }
        Str -> Chars = NewChars;
        Str -> Capacity = NewCapacity;
    }
    for (int x = Str -> Size ; x != Position - 1; x--) {
        Str -> Chars[x + 1] = Str -> Chars[x];
    }
    Str -> Chars[Position] = Char;
    Str -> Size++;
}

void PutsStr(String* Str, const char Char, Usize Position)
{
    Str -> Chars[Position] = Char;
}

Vector* FindStr(String* Str, const char* Char) {
    Vector* Indexs = malloc(sizeof(Vector));
    New(Indexs, sizeof(Usize));

    int SrcSize = strlen(Char);
    Bool ContainsSrc = False;
    for (int x = 0 ; Str -> Chars[x] != '\0' ; x++) {
        if (Str -> Chars[x] == Char[0]) {
            Usize px = x;
            int y = 0;
            int i = x;
            while (Str -> Chars[i] == Char[y] && y < SrcSize && Str -> Chars[i] != '\0') {
                i++;
                y++;
            }
            if (y ==  SrcSize) {
                Usize end = px + y;
                ContainsSrc = True;
                Add(Indexs, &px);
                Add(Indexs, &end);
            }
        }
    }
    return Indexs;
}

void ReplaceStr(String* Str, const char* Char, const char* Replace)
{
    Vector* Indexs = FindStr(Str, Char);
    Usize Matches = Len(Indexs) / 2;
    String* Result = FromStr("");
    Usize x = 0;
    Usize MatchIdx = 0;
    while (x < Str -> Size) {
        while (MatchIdx < Matches) {
            Usize* s = Get(Indexs, MatchIdx * 2);
            if (*s < x) {
                MatchIdx++;
                continue;
            }
            break;
        }
        if (MatchIdx < Matches) {
            Usize* s = Get(Indexs, MatchIdx * 2);
            Usize* e = Get(Indexs, MatchIdx * 2 + 1);
            if (x == *s) {
                AppendStr(Result, (char*)Replace);
                x = *e;
                MatchIdx ++;
                continue;
            }
        }
        AddStr(Result, Str -> Chars[x]);
        x++;
    }
    free(Str -> Chars);
    Str -> Chars = Result -> Chars;
    Str -> Capacity = Result -> Capacity;
    Str -> Size = Result -> Size;
    free(Result);
}

void ReplaceEquallyStr(String* Str, const char* Char, const char* Replace)
{
    Vector* Indexs = FindStr(Str, Char);
    for (Usize x = 0; x < Len(Indexs); x+=2) {
        Usize* s = Get(Indexs, x);
        Usize* e = Get(Indexs, x + 1);
        Usize w = 0;
        for (int z = *s ; z != *e; z++) {
            PutsStr(Str, Replace[w], z);
            w++;
        }
    }
}

char* Cstring(String* Str) {
    return Str -> Chars;
}

String* ConcatStr(String* Str, String* Src) {
    String* TemporaryString = FromStr("");
    for (int x = 0 ; x < Str -> Size; x++) {
        AddStr(TemporaryString, Str -> Chars[x]);
    }
    for (int x = 0 ; x < Src -> Size; x++) {
        AddStr(TemporaryString, Src -> Chars[x]);
    }
    return TemporaryString;
}

Bool IsSpace(const char Char) 
{
    if (Char == ' ' || Char == '\n' || Char == '\t') 
    {
        return True;
    }
    return False;
}
