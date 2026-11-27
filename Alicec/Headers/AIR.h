#ifndef _IntermediateRepresentationH_
#define _IntermediateRepresentationH_

#include "Headers/AliceTypes.h"
#include "Headers/Parser.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/ForwardInclude.h"

#define __EntryBlockMacro "entry"

extern Usize AliceValueIDManager;
extern Usize AliceInstructionIDManager;

typedef struct Struct__AliceIRValue AliceIRValue;
typedef struct Struct__AliceInstructionType AliceInstructionType;
typedef struct Struct__AliceIRInstruction AliceIRInstruction;
typedef struct Struct__AliceIRBlock AliceIRBlock;
typedef struct Struct__AliceIRFunction AliceIRFunction;
typedef struct Struct__AliceIRModule AliceIRModule;
typedef struct Struct__AliceIRBuilder AliceIRBuilder;
typedef struct Struct__AliceIRContext AliceIRContext;
typedef struct Struct__AliceIRSlot AliceIRSlot;

typedef enum Enum__AliceIRValueKind
{
    IRValue_None,
    IRValue_Constant,
    IRValue_Temporary,
    IRValue_Slot,
    IRValue_Type
} ALiceIRValueKind;

#define __OpCode(K) extern const AliceInstructionType ConstantPointerTo##K;
typedef enum Enum__AliceInstructionOpcodeKind
{
    AliceIROpcode_IAddition = -1,
    AliceIROpcode_ISubtraction = -2,
    AliceIROpcode_IDivision = -3,
    AliceIROpcode_IMultiplication = -4,
    
    AliceIROpcode_UAddition = 1,
    AliceIROpcode_USubtraction = 2,
    AliceIROpcode_UDivision = 3,
    AliceIROpcode_UMultiplication = 4,

    AliceIROpcode_Store,
    AliceIROpcode_Load,
    AliceIROpcode_Return = 256,
    AliceIROpcode_ReturnNull,
    AliceIROpcode_Go,
    AliceIROpcode_ConditionalGo,
    AliceIROpcode_Unreachable
} AliceInstructionOpcodeKind;

__OpCode(AliceIROpcode_IAddition)
__OpCode(AliceIROpcode_ISubtraction)
__OpCode(AliceIROpcode_IDivision)
__OpCode(AliceIROpcode_IMultiplication)
__OpCode(AliceIROpcode_UAddition)
__OpCode(AliceIROpcode_USubtraction)
__OpCode(AliceIROpcode_UDivision)
__OpCode(AliceIROpcode_UMultiplication)
__OpCode(AliceIROpcode_Store)
__OpCode(AliceIROpcode_Load)
__OpCode(AliceIROpcode_Return)
__OpCode(AliceIROpcode_ReturnNull)
__OpCode(AliceIROpcode_Go)
__OpCode(AliceIROpcode_ConditionalGo)
__OpCode(AliceIROpcode_Unreachable)

struct Struct__AliceIRValue
{
    Usize ID;
    ALiceIRValueKind Kind;

    AliceNumericType NumType;
    Dsize IntegerValue;
    double FloatingValue;
    __AliceType* Type;
};

struct Struct__AliceInstructionType
{
    AliceInstructionOpcodeKind Kind;
};

struct Struct__AliceIRInstruction
{
    Usize ID;
    const AliceInstructionType* Type;
    AliceIRValue* Primary;
    AliceIRValue* Secondary;
    AliceIRValue* Tertiary;
};

struct Struct__AliceIRModule
{
    __AliceString* Name;
    __AliceVector* FunctionNames; // Vector<__AliceString*>*
    __AliceDictionary* Functions; // Vector<AliceIRFunction*>*
};

struct Struct__AliceIRBlock
{
    __AliceString* Name;
    __AliceStack* Instructions; // Vector<AliceIRInstruction*>*
};

struct Struct__AliceIRFunction
{
    __AliceString* Name;
    __AliceVector* Slots; // Vector<AliceIRSlot*>*
    Usize ParamCount;
    Usize BlockCount;
    Usize TemporaryCount;
    __AliceType* ReturnType;
    __AliceVector* BlockNames;
    __AliceDictionary* Blocks; // Vector<AliceIRBlock*>*
};

struct Struct__AliceIRSlot
{
    __AliceString* Name;
    __AliceType* Type;
};

struct Struct__AliceIRContext
{
    
};

struct Struct__AliceIRBuilder
{
    AliceIRFunction* CurrentFunction;
    AliceIRBlock* CurrentBlock;
    AliceIRModule* CurrentModule;
};

Bool IsTerminatorInstruction(AliceInstructionOpcodeKind Kind);

AliceIRBlock* NewAlIRBlock(CString Name);
AliceIRBlock* NewAlIRUniqueBlock(CString BaseName, AliceIRFunction* Function);
AliceInstructionType* NewAlInstructionType(AliceInstructionOpcodeKind Kind);
AliceIRInstruction* NewAlInstruction(const AliceInstructionType* Type, AliceIRValue* Primary, AliceIRValue* Secondary, AliceIRValue* Tertiary);
AliceIRValue* NewAlIRValue(ALiceIRValueKind Kind, __AliceType* Type);
AliceIRFunction* NewAlIRFunction(CString Name, __AliceType* ReturnType);
AliceIRBuilder* NewAlIRBuilder();
AliceIRContext* NewAlIRContext();
AliceIRModule* NewAlIRModule(CString Name);
AliceIRSlot* NewAlIRSlot(CString Name, __AliceType* Type);

__Void NewAlIRBlockInstruction(AliceIRBlock* Block, AliceIRInstruction* Instruction);
__Void NewAlIRFunctionBlock(AliceIRFunction* Function, AliceIRBlock* Block);
__Void NewAlIRModuleFunction(AliceIRModule* Module, AliceIRFunction* Function);

AliceIRModule* BuildIRFromTypedASTIRBuilder(AliceIRBuilder* Builder, ParserNode* AST);
__Void BuildFromParserNodeIRBuilder(AliceIRBuilder* Builder, ParserNode* AST);
__Void BuildFromFunctionImplIRBuilder(AliceIRBuilder* Builder, FunctionImplDeclaration* N);
__Void BuildFromReturnIRBuilder(AliceIRBuilder* Builder, ReturnDeclaration* N);
static AliceIRValue* BuildFromExpressionIRBuilder(AliceIRBuilder* Builder, Expression* N);
__Void BuildFromVariableAssignIRBuilder(AliceIRBuilder* Builder, VariableAssignDeclaration* N);
__Void BuildFromMemberIRBuilder(AliceIRBuilder* Builder, MemberDeclaration* N);
__Void BuildFromParserNodeIRBuilder(AliceIRBuilder* Builder, ParserNode* N);

Usize NewSlotIRBuilder(AliceIRBuilder* Builder, MemberDeclaration* Member);
__Void NewAlIRInstructionIRBuilder(AliceIRBuilder* Builder, AliceIRInstruction* Instruction);
__Void NewAlIRBlockIRBuilder(AliceIRBuilder* Builder, AliceIRBlock* Block);
__Void NewAlIRFunctionIRBuilder(AliceIRBuilder* Builder, AliceIRFunction* Function);

__Void ConversionToString__IRBuilder(AliceIRBuilder* Builder);
static AliceIRValue* NewAlIRFunctionLoadIRBuilder(AliceIRBuilder* Builder, Usize Slot, __AliceType* Type);
#endif