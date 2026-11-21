#ifndef _IntermediateRepresentationH_
#define _IntermediateRepresentationH_

#include "Headers/AliceTypes.h"
#include "Headers/Parser.h"
#include "Headers/AliceMemoryManagement.h"

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

typedef enum Enum__AliceIRValueKind
{
    IRValue_None,
    IRValue_Constant,
    IRValue_Temporary,
    IRValue_Slot,
    IRValue_Type
} ALiceIRValueKind;

typedef enum Enum__AliceInstructionTypeKind
{
    Addition,
    Subtraction,
    Division,
    Multiplication,
    Return
} AliceInstructionTypeKind;

struct Struct__AliceIRValue
{
    Usize ID;
    ALiceIRValueKind Kind;
    __AliceType* Type;
};

struct Struct__AliceInstructionType
{
    AliceInstructionTypeKind Kind;
};

struct Struct__AliceIRInstruction
{
    Usize ID;
    AliceIRValue* Left;
    AliceIRValue* Right;
    AliceInstructionType* Type;
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
    __AliceVector* BlockNames;
    __AliceDictionary* Blocks; // Vector<AliceIRBlock*>*
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

AliceIRBlock* NewAlIRBlock(__CString Name);
AliceInstructionType* NewAlInstructionType(AliceInstructionTypeKind Kind);
AliceIRInstruction* NewAlInstruction(AliceInstructionType* Type, AliceIRValue* Left, AliceIRValue* Right);
AliceIRValue* NewAlValue(ALiceIRValueKind Kind, __AliceType* Type);
AliceIRFunction* NewAlIRFunction(__CString Name);
AliceIRBuilder* NewAlIRBuilder();
AliceIRContext* NewAlIRContext();
AliceIRModule* NewAlIRModule(__CString Name);

__Void NewAlIRBlockInstruction(AliceIRBuilder* Builder, AliceIRBlock* Block, AliceIRInstruction* Instruction);
__Void NewAlIRFunctionBlock(AliceIRBuilder* Builder, AliceIRFunction* Function, AliceIRBlock* Block);
__Void NewAlIRModuleFunction(AliceIRBuilder* Builder, AliceIRModule* Module, AliceIRFunction* Function);

__Void BuildIRFromTypedASTIRBuilder(AliceIRBuilder* Builder, ParserNode* AST);
__Void BuildFromParserNodeIRBuilder(AliceIRBuilder* Builder, ParserNode* AST);
__Void BuildFromFunctionImplIRBuilder(AliceIRBuilder* Builder, FunctionImplDeclaration* N);
__Void BuildFromReturnIRBuilder(AliceIRBuilder* Builder, ReturnDeclaration* N);
__Void BuildFromExpressionIRBuilder(AliceIRBuilder* Builder, Expression* N);
__Void BuildFromVariableAssignIRBuilder(AliceIRBuilder* Builder, VariableAssignDeclaration* N);
__Void BuildFromMemberIRBuilder(AliceIRBuilder* Builder, MemberDeclaration* N);
__Void BuildFromParserNodeIRBuilder(AliceIRBuilder* Builder, ParserNode* N);

#endif