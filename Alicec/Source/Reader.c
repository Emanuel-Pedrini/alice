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
    CAttributes -> TargetArchive = NULL;
    return CAttributes;
}

static char* CStringWithoutInitialSymbols(CString Source)
{
    Usize Size =  strlen(Source);
    Usize Length = 0;
    for (; Source[Length] != '\0' && !IsIndentifierChar(Source[Length]); Length++){};
    return Substring(Source, Length, Size);
}

__Void ReadCompilationTargetPathCReader(AliceCompilationAttributesReader* AttributesReader, CString Argument)
{
    AttributesReader -> Attributes -> TargetArchive = __AlStringFrom(CStringWithoutInitialSymbols(Argument));
}

__Void ReadCompilationFlagCReader(AliceCompilationAttributesReader* AttributesReader, CString Argument)
{
    __PushAlVector(AttributesReader -> Attributes -> Flags, CStringWithoutInitialSymbols(Argument));
}

__Void ReadNewAliceSourceFile(AliceCompilationAttributesReader* AttributesReader, CString Argument)
{
    __AliceString* NewPath = __AlStringFrom(Argument);
    __PushAlVector(AttributesReader -> Attributes -> Paths, NewPath);
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
        if (CStringStartsWith(Argument, __AliceTargetCompilationPathPrefix))
        {
            ReadCompilationTargetPathCReader(AttributesReader, Argument);
        }
        else if (CStringStartsWith(Argument, __AliceCompilationFlagPrefix))
        {
            ReadCompilationFlagCReader(AttributesReader, Argument);
        }
        else if (CStringEndsWith(Argument, __AliceFileSuffix))
        {
            ReadNewAliceSourceFile(AttributesReader, Argument);
        }
        else 
        {
            Interator++;
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