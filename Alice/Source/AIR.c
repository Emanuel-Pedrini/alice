#include "Headers/AIR.h"
Usize AliceValueIDManager = 0;
Usize AliceInstructionIDManager = 0;

AliceIRValue* NewAlValue(ALiceIRValueKind Kind, __AliceType* Type)
{
    AliceValueIDManager++;
    AliceIRValue* Value = __AlAllocate(sizeof(AliceIRValue));
    Value -> ID = AliceValueIDManager;
    Value -> Kind = Kind;
    Value -> Type = Type;
    return Value;
}

AliceInstructionType* NewAlInstructionType(AliceInstructionTypeKind Kind)
{
    AliceInstructionType* InstructionType = __AlAllocate(sizeof(AliceInstructionType));
    InstructionType -> Kind = Kind;
    return InstructionType;
}

AliceIRInstruction* NewAlInstruction(AliceInstructionType* Type, AliceIRValue* Left, AliceIRValue* Right)
{
    AliceInstructionIDManager++;
    AliceIRInstruction* Instruction = __AlAllocate(sizeof(AliceIRInstruction));
    Instruction -> Type = Type;
    Instruction -> Left = Left;
    Instruction -> Right = Right;
    Instruction -> ID = AliceInstructionIDManager;
    return Instruction;
}

AliceIRBlock* NewAlIRBlock(__CString Name)
{
    
    AliceIRBlock* Block = __AlAllocate(sizeof(AliceIRBlock));
    Block -> Instructions = __NewAlStack(sizeof(AliceIRInstruction*));
    Block -> Name = __AlStringFrom(Name);
    return Block;
}


AliceIRFunction* NewAlIRFunction(__CString Name)
{
    AliceIRFunction* Function = __AlAllocate(sizeof(AliceIRFunction));
    Function -> Name = __AlStringFrom(Name);
    Function -> BlockNames = __NewAlVector(sizeof(__AliceString*));
    Function -> Blocks = __NewAlDictionary(sizeof(AliceIRBlock));
    return Function;
}

AliceIRModule* NewAlIRModule(__CString Name)
{
    AliceIRModule* Module = __AlAllocate(sizeof(AliceIRModule));
    Module -> Name = __AlStringFrom(Name);
    Module -> Functions = __NewAlDictionary(sizeof(AliceIRFunction));
    Module -> FunctionNames = __NewAlVector(sizeof(__AliceString*));
    return Module;
}

AliceIRBuilder* NewAlIRBuilder()
{
    AliceIRBuilder* Builder = __AlAllocate(sizeof(AliceIRBuilder));
    Builder -> CurrentModule = NewAlIRModule("root");
    Builder -> CurrentBlock = NULL;
    Builder -> CurrentFunction = NULL;
    return Builder;
}

AliceIRContext* NewAlIRContext()
{
    // Not relevant for now
}


__Void NewAlIRBlockInstruction(AliceIRBuilder* Builder, AliceIRBlock* Block, AliceIRInstruction* Instruction)
{
    __PushAlStack(Block -> Instructions, Instruction);
}

__Void NewAlIRFunctionBlock(AliceIRBuilder* Builder, AliceIRFunction* Function, AliceIRBlock* Block)
{
    __NewElementAlDictionary(Function -> Blocks, Block -> Name, Block);
    __PushAlVector(Function -> BlockNames, Block -> Name);
}

__Void NewAlIRModuleFunction(AliceIRBuilder* Builder, AliceIRModule* Module, AliceIRFunction* Function)
{
    __NewElementAlDictionary(Module -> Functions, Function -> Name, Function);
    __PushAlVector(Module -> FunctionNames, Function -> Name);
}


__Void BuildIRFromTypedASTIRBuilder(AliceIRBuilder* Builder, ParserNode* AST)
{

}

__Void BuildFromBodyIRBuilder(AliceIRBuilder* Builder, BodyDeclaration* N)
{
    __AliceVector* Statements = N -> BodyStatements;
    for (Usize Interator = 0;
        Interator < Statements -> VSize;
        Interator++)
    {
        ParserNode* Limb = ((ParserNode*)__GetAlVector(Statements, Interator));
        BuildFromParserNodeIRBuilder(Builder, Limb);
    }
}

__Void BuildFromFunctionImplIRBuilder(AliceIRBuilder* Builder, FunctionImplDeclaration* N)
{

}

__Void BuildFromReturnIRBuilder(AliceIRBuilder* Builder, ReturnDeclaration* N)
{

}

__Void BuildFromExpressionIRBuilder(AliceIRBuilder* Builder, Expression* N)
{

}

__Void BuildFromVariableAssignIRBuilder(AliceIRBuilder* Builder, VariableAssignDeclaration* N)
{

}

__Void BuildFromMemberIRBuilder(AliceIRBuilder* Builder, MemberDeclaration* N)
{

}

__Void BuildFromParserNodeIRBuilder(AliceIRBuilder* Builder, ParserNode* N)
{
    switch (N -> Type)
    {
        case AsBody: 
        {
            BuildFromBodyIRBuilder(Builder, &(N -> Declaration.FieldBody));
            break;
        }
        case AsFunctionImpl:
        {
            BuildFromFunctionImplIRBuilder(Builder, &(N -> Declaration.FieldFunctionImpl));
            break;
        }
        case AsReturn:
        {
            BuildFromReturnIRBuilder(Builder, &(N -> Declaration.FieldReturn));
            break;
        }
        case AsExpression:
        {
            BuildFromExpressionIRBuilder(Builder, &(N -> Declaration.FieldExpression));break;}
        case AsVariableAssign:
        {
            BuildFromVariableAssignIRBuilder(Builder, &(N -> Declaration.FieldVariableAssign));
            break;
        }
        case AsMember:
        {
            BuildFromMemberIRBuilder(Builder, &(N -> Declaration.FieldMember));
            break;
        }
    }
}