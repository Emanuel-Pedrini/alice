#include "Compiler/Native/AliceLLVMAPI.h"

int GenerateObjectFileFromLLVMIR(CString LLVMIR, CString OutputPath, CString Passes, Bool SpecificCpu)
{
    size_t LLVMIRLength = strlen(LLVMIR);
    int Record = 1;
    char* Error = NULL;
    char* TargetTriple = NULL;

    LLVMModuleRef AliceLLVMModule = NULL;
    LLVMTargetRef AliceLLVMTarget = NULL;
    LLVMTargetMachineRef AliceLLVMTargetMachine = NULL;
    LLVMPassBuilderOptionsRef AliceLLVMPassBuilderOptions = NULL;
    LLVMTargetDataRef AliceLLVMLayout = NULL;
    LLVMErrorRef AliceLLVMError = NULL;

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    LLVMContextRef AliceLLVMContext = LLVMContextCreate();
    LLVMMemoryBufferRef AliceLLVMMemoryBuffer = LLVMCreateMemoryBufferWithMemoryRangeCopy(LLVMIR, LLVMIRLength, "module");

    if (LLVMParseIRInContext(AliceLLVMContext, AliceLLVMMemoryBuffer, &AliceLLVMModule, &Error)) 
    {
        fprintf(stderr, "LLVM Error, %s!", Error);
        goto ___LabelProcessDone;
    }

    if (LLVMVerifyModule(AliceLLVMModule, LLVMPrintMessageAction, NULL)) goto ___LabelProcessDone;
    TargetTriple = LLVMGetDefaultTargetTriple();

    if (LLVMGetTargetFromTriple(TargetTriple, &AliceLLVMTarget, &Error)) 
    {
        fprintf(stderr, "LLVM Error, %s!", Error);
        goto ___LabelProcessDone;
    }

    if (!SpecificCpu) AliceLLVMTargetMachine = LLVMCreateTargetMachine(AliceLLVMTarget, TargetTriple, "generic", "", LLVMCodeGenLevelDefault, LLVMRelocPIC, LLVMCodeModelDefault);
    else AliceLLVMTargetMachine = LLVMCreateTargetMachine(AliceLLVMTarget, TargetTriple, LLVMGetHostCPUName(), LLVMGetHostCPUFeatures(), LLVMCodeGenLevelDefault, LLVMRelocPIC, LLVMCodeModelDefault);

    LLVMSetTarget(AliceLLVMModule, TargetTriple);
    AliceLLVMLayout = LLVMCreateTargetDataLayout(AliceLLVMTargetMachine);
    LLVMSetModuleDataLayout(AliceLLVMModule, AliceLLVMLayout);
    LLVMDisposeTargetData(AliceLLVMLayout);
    AliceLLVMPassBuilderOptions = LLVMCreatePassBuilderOptions();

    AliceLLVMError = LLVMRunPasses(AliceLLVMModule, Passes, AliceLLVMTargetMachine, AliceLLVMPassBuilderOptions);
    if (AliceLLVMError) 
    {
        char* AliceLLVMErrorMessage = LLVMGetErrorMessage(AliceLLVMError);
        LLVMDisposeErrorMessage(AliceLLVMErrorMessage);
        goto ___LabelProcessDone;
    }

    if (LLVMTargetMachineEmitToFile(AliceLLVMTargetMachine, AliceLLVMModule, (char*)OutputPath, LLVMObjectFile, &Error))
    {
        fprintf(stderr, "LLVM Error, %s!", Error); 
        goto ___LabelProcessDone;
    }

    Record = 0;

    ___LabelProcessDone:
        if (Error) LLVMDisposeMessage(Error);
        if (TargetTriple) LLVMDisposeMessage(TargetTriple);
        if (AliceLLVMPassBuilderOptions) LLVMDisposePassBuilderOptions(AliceLLVMPassBuilderOptions);
        if (AliceLLVMTargetMachine) LLVMDisposeTargetMachine(AliceLLVMTargetMachine);
        if (AliceLLVMModule) LLVMDisposeModule(AliceLLVMModule);
        LLVMContextDispose(AliceLLVMContext);
        return Record;
}

int GenerateExecutableFromObjectFile(CString LLVMIR, CString OutputPath, CString Passes, Bool SpecificCpu, Bool KeepTemporaries)
{
    char ObjectFilePath[1024];
    snprintf(ObjectFilePath, sizeof(ObjectFilePath), "%s.o", OutputPath);
    if (GenerateObjectFileFromLLVMIR(LLVMIR, OutputPath, Passes, SpecificCpu) != 0)
    {
        remove(ObjectFilePath);
        return 1;
    }
    char Command[2048];
    snprintf(Command, sizeof(Command), "clang \"%s\" -o \"%s\"", ObjectFilePath, OutputPath);
    int Result = system(Command);
    if (!KeepTemporaries) remove(ObjectFilePath);
    return Result;
}