#include "External/Libraries.h"
#include "Headers/ArchiveManager.h"
#include "Headers/CStringUtils.h"
#include "Headers/InterpreterConstants.h"
#include "Headers/Lexer.h"
#include "Headers/Reader.h"
#include "Headers/Lexer.h"

AliceCompilationAttributesReader* NewAliceCompilationReader()
{
    AliceCompilationAttributesReader* CAttributesReader = __AlAllocate(sizeof(AliceCompilationAttributesReader));
    CAttributesReader -> Arguments = NULL;
    CAttributesReader -> ArgumentsCount = 0;
    CAttributesReader -> Pointer = 0;
    CAttributesReader -> Attributes = NewAliceCompilationAttributes();
    CAttributesReader -> ManagementObject = NewAliceManagementObject();
    return CAttributesReader;
}

AliceCompilationAttributes* NewAliceCompilationAttributes()
{
    AliceCompilationAttributes* CAttributes = __AlAllocate(sizeof(AliceCompilationAttributes));
    CAttributes -> Paths = __NewAlVector(sizeof(__AliceString*));
    CAttributes -> Specifications = __NewAlVector(sizeof(Placeholder));
    CAttributes -> Flags = __NewAlVector(sizeof(CString));
    CAttributes -> FlagValues = __NewAlDictionary(sizeof(__AliceString*));
    CAttributes -> TargetArchive = NULL;
    return CAttributes;
}

static char* CStringWithoutInitialSymbols(CString Source)
{
    Usize Size =  strlen(Source);
    Usize Length = 0;
    for (; Source[Length] != '\0' && !IsIdentifierChar(Source[Length]); Length++){};
    return Substring(Source, Length, Size);
}

__Void ReadCompilationTargetPathCReader(AliceCompilationAttributesReader* AttributesReader, CString Argument)
{
    AttributesReader -> Attributes -> TargetArchive = __AlStringFrom(CStringWithoutInitialSymbols(Argument));
}

__Void ReadCompilationFlagCReader(AliceCompilationAttributesReader* AttributesReader, CString Argument)
{
    char* FlagTotal = CStringWithoutInitialSymbols(Argument);
    int Size = strlen(FlagTotal);
    int i = 0;
    for (; FlagTotal[i] != '=' && FlagTotal[i] != '\0'; i++){}
    
    __AliceString* FlagName = __AlStringFrom(Substring(FlagTotal, 0, i));
    __AliceString* FlagValue = __AlStringFrom(FlagTotal[i] == '=' ? (FlagTotal + i + 1) : "");

    if (__AlStringCompare(FlagName, __AlStringFrom("out"))) AttributesReader -> Attributes -> TargetArchive = FlagValue;

    __PushAlVector(AttributesReader -> Attributes -> Flags, FlagName);
    __NewElementAlDictionary(AttributesReader -> Attributes -> FlagValues, FlagName, FlagValue);
}

__Void ReadAliceSourceFile(AliceCompilationAttributesReader* AttributesReader, CString Argument)
{
    __AliceString* NewPath = __AlStringFrom(Argument);
    __PushAlVector(AttributesReader -> Attributes -> Paths, NewPath);
}
__Void ReadMultipleAliceSourceFile(AliceCompilationAttributesReader* AttributesReader, CString Argument)
{
    int SuffixSize = strlen(__AliceMultipleIncludeSuffix);
    int ArgSize = strlen(Argument);
    int Offset = 1;
    char Last = Argument[ArgSize - (SuffixSize + 1)];
    Bool IsSeparator = ((Last == '\\') || (Last == '/'));
    if (Last && IsSeparator)
    {
        char* OnlyPath = Substring(Argument, 0, ArgSize - (SuffixSize + Offset));
        __AliceVector* Buffer = __NewAlVector(sizeof(__AliceString*));
        ListDirectory(OnlyPath, ".al", Buffer);
        for (int i = 0;i < Buffer -> VSize;i++)
        {
            char* SourceFile = ((__AliceString*)__GetAlVector(Buffer, i)) -> Chars;
            ReadAliceSourceFile(AttributesReader, SourceFile);
        }
    }
}

AliceCompilationAttributes* ReadCompilationAttributes(AliceCompilationAttributesReader* AttributesReader, const int NumberOfArguments, CString* Arguments)
{
    AttributesReader -> Arguments = Arguments;
    AttributesReader -> ArgumentsCount = NumberOfArguments;
    for (Usize Interator = 1;
        Interator < AttributesReader -> ArgumentsCount;
        Interator++)
    {
        CString Argument = AttributesReader -> Arguments[Interator];
        if (CStringStartsWith(Argument, __AliceCompilationFlagPrefix))
        {
            ReadCompilationFlagCReader(AttributesReader, Argument);
        }
        else if (CStringEndsWith(Argument, __AliceMultipleIncludeSuffix))
        {
            ReadMultipleAliceSourceFile(AttributesReader, Argument);
        }
        else if (CStringEndsWith(Argument, __AliceFileSuffix))
        {
            ReadAliceSourceFile(AttributesReader, Argument);
        }
    }
    return AttributesReader -> Attributes;
}

Bool ReadCompilationPaths(AliceCompilationAttributesReader* AttributesReader)
{
    __AliceVector* CompilationPaths = AttributesReader -> Attributes -> Paths;
    for (Usize Interator = 0; Interator < CompilationPaths -> VSize; Interator++)
    {
        __AliceString* NewPath = (__AliceString*)(__GetAlVector(CompilationPaths, Interator));
        AliceCodeObject* NewCode = OpenAliceFile(NewPath -> Chars);
        if (NewCode == NULL) return False;
        IncludeManagementObject(AttributesReader -> ManagementObject, NewCode);
    }
    return True;
}

AliceManagementObject* ManageCompilationObject(AliceCompilationAttributesReader* AttributesReader)
{
    ReadCompilationPaths(AttributesReader);
    return AttributesReader -> ManagementObject;
}