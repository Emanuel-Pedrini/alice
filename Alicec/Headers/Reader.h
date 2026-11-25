#ifndef _ReaderH_
#define _ReaderH_

#include "Headers/AliceMemoryManagement.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/ForwardInclude.h"

#define __AliceFileSuffix ".al"
#define __AliceTargetCompilationPathPrefix "--out="
#define __AliceMultipleIncludeSuffix "*.al"
#define __AliceCompilationFlagPrefix "--"

struct Struct__AliceCompilationAttributesReader
{
    Usize Pointer;
    Usize ArgumentsCount;
    CString* Arguments;
    AliceManagementObject* ManagementObject;
    AliceCompilationAttributes* Attributes;
};

struct Struct__AliceCompilationAttributes 
{
    __AliceVector* Specifications;
    __AliceString* TargetArchive;
    __AliceVector* Flags;
    __AliceDictionary* FlagValues;
    __AliceVector* Paths;
};

AliceCompilationAttributesReader* NewAliceCompilationReader();
AliceCompilationAttributes* NewAliceCompilationAttributes();

AliceManagementObject* ManageCompilationObject(AliceCompilationAttributesReader* AttributesReader);
AliceCompilationAttributes* ReadCompilationAttributes(AliceCompilationAttributesReader* AttributesReader, const int NumberOfArguments, CString* Arguments);

Bool ReadCompilationPaths(AliceCompilationAttributesReader* AttributesReader);

__Void ReadAliceSourceFile(AliceCompilationAttributesReader* AttributesReader, CString Argument);
__Void ReadMultipleAliceSourceFile(AliceCompilationAttributesReader* AttributesReader, CString Argument);
__Void ReadCompilationFlagCReader(AliceCompilationAttributesReader* AttributesReader, CString Argument);
__Void ReadCompilationTargetPathCReader(AliceCompilationAttributesReader* AttributesReader, CString Argument);

#endif