#include "Compiler/Native/AliceLLVM.h"

AliceCompilerResult* CompileToLLVMFromAIR(AliceCodeWritter* Writter, AliceIRModule* Module)
{
    CompileIRModuleFromAIR(Writter, Module);
    return NewAliceCompilerResult(True, CodeGenerateCodeWritter(Writter));
}

__Void CompileIRModuleFromAIR(AliceCodeWritter* Writter, AliceIRModule* Module)
{
    for 
    (
        Usize Interator = 0;
        Interator < Module -> FunctionNames -> VSize;
        Interator += 1
    )
    {
        __AliceString* ChildFunctionName = (*(__AliceString**)__GetAlVector(Module -> FunctionNames, Interator));
        AliceIRFunction* ChildFunction = (*(AliceIRFunction**)__GetAlDictionary(Module -> Functions, ChildFunctionName));
        CompileIRFunctionFromAIR(Writter, ChildFunction);
    }
}

__Void CompileIRFunctionFromAIR(AliceCodeWritter* Writter, AliceIRFunction* Function)
{
    char B[128];
    snprintf(B, sizeof(B), "define i32 @%s() {\n", Function -> Name -> Chars);
    AliceCodeBlock* Bl = NewCodeBlockFormatted(B, "\n}");
    Writter -> CurrentBlock = Bl;
    NewCodeWriterBlock(Writter, Bl);
    for 
    (
        Usize Interator = 0;
        Interator < Function -> BlockCount;
        Interator += 1
    )
    {
        __AliceString* ChildBlockName = (*(__AliceString**)__GetAlVector(Function -> BlockNames, Interator));
        AliceIRBlock* ChildBlock = (*(AliceIRBlock**)__GetAlDictionary(Function -> Blocks, ChildBlockName));
        CompileIRBlockFromAIR(Writter, ChildBlock);
    }
}

__Void CompileIRBlockFromAIR(AliceCodeWritter* Writter, AliceIRBlock* Block)
{
    AliceCodeBlock* Caller = Writter -> CurrentBlock;
    char B[128];
    snprintf(B, sizeof(B), "%s:\n", Block -> Name -> Chars);
    AliceCodeBlock* Bl = NewCodeBlockFormatted(B, "\n");
    NewCodeBlockBlock(Writter -> CurrentBlock, Bl);
    Writter -> CurrentBlock = Bl;
    for 
    (
        Usize Interator = 0;
        Interator < Block -> Instructions -> Elements -> VSize;
        Interator += 1
    )
    {
        AliceIRInstruction* ChildInstruction = (*(AliceIRInstruction**)__GetAlVector(Block -> Instructions -> Elements, Interator));
        CompileIRInstructionFromAIR(Writter, ChildInstruction);
    }
    Writter -> CurrentBlock = Caller;
}

__Void CompileIRInstructionFromAIR(AliceCodeWritter* Writter, AliceIRInstruction* Instruction)
{
    AliceCodeBlock* Caller = Writter -> CurrentBlock;
    char B[256] = "";
    switch (Instruction -> Type -> Kind)
    {
        case AliceIROpcode_Return:
            snprintf(B, sizeof(B), "ret %s", CompileIRValueFromAIR(Writter, Instruction -> Primary) -> Chars);
            AliceCodeLine* L = NewCodeLine(B);
            NewCodeBlockLine(Writter -> CurrentBlock, L);
            break;
        
        default:
            break;
    }
    Writter -> CurrentBlock = Caller;
}

__AliceString* CompileIRValueFromAIR(AliceCodeWritter* Writter, AliceIRValue* Value)
{
    char B[128] = "";
    switch (Value -> Kind)
    {
        case IRValue_Constant:
            switch (Value -> NumType)
            {
                case AlNumericType_Integer:
                    snprintf(B, sizeof(B), "%s%d %d", 
                    Value -> Type -> Information.Numeric.IsSigned ? "i" : "u", 
                    Value -> Type -> Information.Numeric.BitWidth, 
                    Value -> IntegerValue);
                    
                    return __AlStringFrom(B);
                
                default:
                    break;
            }
        default:
            break;
    }
}