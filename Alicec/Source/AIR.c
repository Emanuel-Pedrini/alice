#include "Headers/AIR.h"
Usize AliceValueIDManager = 0;
Usize AliceInstructionIDManager = 0;

#define __OpCodePtr(K) const AliceInstructionType ConstantPointerTo##K = { K };
__OpCodePtr(AliceIROpcode_IAddition)
__OpCodePtr(AliceIROpcode_ISubtraction)
__OpCodePtr(AliceIROpcode_IDivision)
__OpCodePtr(AliceIROpcode_IMultiplication)
__OpCodePtr(AliceIROpcode_UAddition)
__OpCodePtr(AliceIROpcode_USubtraction)
__OpCodePtr(AliceIROpcode_UDivision)
__OpCodePtr(AliceIROpcode_UMultiplication)
__OpCodePtr(AliceIROpcode_Store)
__OpCodePtr(AliceIROpcode_Load)
__OpCodePtr(AliceIROpcode_Return)
__OpCodePtr(AliceIROpcode_ReturnNull)
__OpCodePtr(AliceIROpcode_Go)
__OpCodePtr(AliceIROpcode_ConditionalGo)
__OpCodePtr(AliceIROpcode_Unreachable)

Bool IsTerminatorInstruction(AliceInstructionOpcodeKind Kind)
{
    return (Bool)(Kind > 255);
}
AliceIRValue* NewAlIRValue(ALiceIRValueKind Kind, __AliceType* Type)
{
    AliceValueIDManager++;
    AliceIRValue* Value = __AlAllocate(sizeof(AliceIRValue));
    Value -> ID = AliceValueIDManager;
    Value -> Kind = Kind;
    Value -> Type = Type;
    return Value;
}

__AliceString* ConversionToString__AliceIRValue(AliceIRValue* AlValue)
{

}

AliceInstructionType* NewAlInstructionType(AliceInstructionOpcodeKind Kind)
{
    AliceInstructionType* InstructionType = __AlAllocate(sizeof(AliceInstructionType));
    InstructionType -> Kind = Kind;
    return InstructionType;
}

AliceIRInstruction* NewAlInstruction(const AliceInstructionType* Type, AliceIRValue* Left, AliceIRValue* Right)
{
    AliceInstructionIDManager++;
    AliceIRInstruction* Instruction = __AlAllocate(sizeof(AliceIRInstruction));
    Instruction -> Type = Type;
    Instruction -> Left = Left;
    Instruction -> Right = Right;
    Instruction -> ID = AliceInstructionIDManager;
    Instruction -> Destiny = NULL;
    return Instruction;
}

AliceIRBlock* NewAlIRBlock(CString Name)
{
    
    AliceIRBlock* Block = __AlAllocate(sizeof(AliceIRBlock));
    Block -> Instructions = __NewAlStack(sizeof(AliceIRInstruction*));
    Block -> Name = __AlStringFrom(Name);
    return Block;
}

AliceIRBlock* NewAlIRUniqueBlock(CString BaseName, AliceIRFunction* Function)
{
    char Buffer[32];
    snprintf(Buffer, sizeof(Buffer), ".%d", Function -> BlockCount);
    __AliceString* Name = __AlStringFrom(BaseName);
    __AppendAlString(Name, Buffer);
    AliceIRBlock* UBlock = NewAlIRBlock(Name -> Chars);
    NewAlIRFunctionBlock(Function, UBlock);
    return UBlock;
}


AliceIRFunction* NewAlIRFunction(CString Name, __AliceType* ReturnType)
{
    AliceIRFunction* Function = __AlAllocate(sizeof(AliceIRFunction));
    Function -> Name = __AlStringFrom(Name);
    Function -> BlockNames = __NewAlVector(sizeof(__AliceString*));
    Function -> Blocks = __NewAlDictionary(sizeof(AliceIRBlock*));
    Function -> Slots = __NewAlVector(sizeof(AliceIRSlot*));
    Function -> BlockCount = 0;
    Function -> TemporaryCount = 0;
    Function -> ParamCount = 0;
    Function -> ReturnType = ReturnType;
    return Function;
}

AliceIRModule* NewAlIRModule(CString Name)
{
    AliceIRModule* Module = __AlAllocate(sizeof(AliceIRModule));
    Module -> Name = __AlStringFrom(Name);
    Module -> Functions = __NewAlDictionary(sizeof(AliceIRFunction*));
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

AliceIRSlot* NewAlIRSlot(CString Name, __AliceType* Type)
{
    AliceIRSlot* Slot = __AlAllocate(sizeof(AliceIRSlot));
    Slot -> Name = __AlStringFrom(Name);
    Slot -> Type = Type;
    return Slot;
}


__Void NewAlIRBlockInstruction(AliceIRBlock* Block, AliceIRInstruction* Instruction)
{
    __PushAlStack(Block -> Instructions, Instruction);
}

__Void NewAlIRFunctionBlock(AliceIRFunction* Function, AliceIRBlock* Block)
{
    __NewElementAlDictionary(Function -> Blocks, Block -> Name, Block);
    __PushAlVector(Function -> BlockNames, Block -> Name);
    Function -> BlockCount++;
}

__Void NewAlIRModuleFunction(AliceIRModule* Module, AliceIRFunction* Function)
{
    __NewElementAlDictionary(Module -> Functions, Function -> Name, Function);
    __PushAlVector(Module -> FunctionNames, Function -> Name);
}

Usize NewSlotIRBuilder(AliceIRBuilder* Builder, MemberDeclaration* Member)
{
    AliceIRFunction* Function = Builder -> CurrentFunction;
    AliceIRSlot* Slot = NewAlIRSlot(Member -> Name -> Chars, Member -> Type);
    Usize ID = Function -> Slots -> VSize;
    __PushAlVector(Function -> Slots, Slot);
    Member -> Symbol -> Slot = ID;
    return ID;
}

__Void NewAlIRInstructionIRBuilder(AliceIRBuilder* Builder, AliceIRInstruction* Instruction)
{
    AliceIRBlock* Block = Builder -> CurrentBlock;
    NewAlIRBlockInstruction(Block, Instruction);
}

__Void NewAlIRBlockIRBuilder(AliceIRBuilder* Builder, AliceIRBlock* Block)
{
    AliceIRFunction* Function = Builder -> CurrentFunction;
    Builder -> CurrentBlock = Block;
    NewAlIRFunctionBlock(Function, Block);
}

__Void NewAlIRFunctionIRBuilder(AliceIRBuilder* Builder, AliceIRFunction* Function)
{
    AliceIRModule* Module = Builder -> CurrentModule;
    Builder -> CurrentFunction = Function;
    NewAlIRModuleFunction(Module, Function);
}

AliceIRModule* BuildIRFromTypedASTIRBuilder(AliceIRBuilder* Builder, ParserNode* AST)
{
    BuildFromParserNodeIRBuilder(Builder, AST);
    return Builder -> CurrentModule;
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
    AliceIRFunction* Function = NewAlIRFunction(N -> Head -> Name -> Chars, N -> Head -> ReturnType);
    AliceIRBlock* EntryBlock = NewAlIRBlock(__EntryBlockMacro);
    NewAlIRFunctionIRBuilder(Builder, Function);
    NewAlIRBlockIRBuilder(Builder, EntryBlock);

    // Handling the slots
    for (Usize Interator = 0; Interator < N -> Head -> Arguments -> Members -> VSize; Interator++)
    {
        MemberDeclaration* Member = ((MemberDeclaration*)__GetAlVector(N -> Head -> Arguments -> Members, Interator));
        NewSlotIRBuilder(Builder, Member);
    }
    Function -> ParamCount = N -> Head -> Arguments -> Members -> VSize;
    BuildFromBodyIRBuilder(Builder, N -> Body);
}

__Void BuildFromReturnIRBuilder(AliceIRBuilder* Builder, ReturnDeclaration* N)
{
    NewAlIRInstructionIRBuilder(
        Builder, 
        NewAlInstruction(
            &ConstantPointerToAliceIROpcode_Return,
            BuildFromExpressionIRBuilder(Builder, N -> Value),
            NULL
        )
    );
}

static AliceIRValue* NewAlIRFunctionLoadIRBuilder(AliceIRBuilder* Builder, Usize Slot, __AliceType* Type)
{
    AliceIRValue* Source = NewAlIRValue(IRValue_Slot, NULL);
    Source -> ID = Slot;
    AliceIRValue* Destiny = NewAlIRValue(IRValue_Temporary, Type);
    NewAlIRInstructionIRBuilder(
        Builder,
        NewAlInstruction(&ConstantPointerToAliceIROpcode_Load, Source, Destiny) 
    );
    return Destiny;
}

static AliceIRValue* BuildFromExpressionIRBuilder(AliceIRBuilder* Builder, Expression* N)
{
    switch (N -> Type)
    {
        case LITERAL:
            switch (N -> Value.Literal -> Type)
            {
                case Tk_Number:
                    AliceIRValue* Value  = NewAlIRValue(IRValue_Constant, N -> ResolvedType);
                    long NumericValue = __StringToInt(__AlStringFrom(N -> Value.Literal->Literal), 10);
                    Value -> IntegerValue = NumericValue;
                    return Value;
                case Tk_Decimal: // Here, change after.
                    return NewAlIRValue(IRValue_Constant, N -> ResolvedType);
                case Tk_Indentifier:
                    return NewAlIRFunctionLoadIRBuilder(Builder, N -> Symbol -> Slot, N -> ResolvedType);
                default:
                    break;
            }
            break;
        case UNARY:
            break;
        case BINARY:
            AliceIRValue* Left = BuildFromExpressionIRBuilder(Builder, N -> Value.Binary -> Left);
            AliceIRValue* Right = BuildFromExpressionIRBuilder(Builder, N -> Value.Binary -> Right);
            break;
        default:
            break;
    }
}

__Void BuildFromVariableAssignIRBuilder(AliceIRBuilder* Builder, VariableAssignDeclaration* N)
{

}

__Void BuildFromMemberIRBuilder(AliceIRBuilder* Builder, MemberDeclaration* N)
{

}

__Void ConversionToString__IRBuilder(AliceIRBuilder* Builder)
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