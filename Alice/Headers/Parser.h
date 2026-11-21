#ifndef _ParserH_
#define _ParserH_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/PrattParsing.h"
#include "Headers/Lexer.h"
#include "Headers/AliceTypes.h"
#include "Headers/ForwardInclude.h"

#define MEMBER_DELIMITER Tk_Colon
#define ARGUMENT_DELIMITER Tk_Comma
#define ASSOCIATIVE_DELIMITER Tk_Equal
#define POINTER_DELIMITER Tk_Commercial

enum Enum__ParserNodeType 
{
    NodeFunction
};

struct Struct__MemberDeclaration // A Common type of declaration
{
    __AliceString* Name;
    __AliceType* Type;
    LocalSymbol* Symbol;
};

struct Struct__MemberGroupDeclaration
{
    __AliceVector* Members; // Vector<MemberDeclaration>*
};

struct Struct__CallArgumentsDeclaration
{
    __AliceVector* Arguments;
};

struct Struct__FlagDeclaration 
{
    __AliceString* Name;
    CallArgumentsDeclaration* CallArguments; 
};

struct Struct__ReturnDeclaration
{
    Expression* Value;
};

struct Struct__FunctionHeadDeclaration
{
    __AliceString* Name;
    __AliceType* ReturnType;
    __AliceVector* Flags;
    MemberGroupDeclaration* Arguments; 
};

struct Struct__BodyDeclaration 
{
    __AliceVector* BodyStatements; // Vector<ParserNode*>*
};

struct Struct__FunctionImplDeclaration 
{
    FunctionHeadDeclaration* Head;
    BodyDeclaration* Body;
};

struct Struct__FunctionCallDeclaration
{
    __AliceString* FunctionName;
    CallArgumentsDeclaration* CallArguments;
};

struct Struct__VariableAssignDeclaration
{
    MemberDeclaration* Member;
    __AliceVector* Flags; // Vector<FlagDeclaration>*
    Expression* Value;
};

struct Struct__MethodCallDeclaration
{
    ParserNodeDeclaration* Object;
    FunctionCallDeclaration* MethodAsFunction;
};

struct  Struct__FieldAcessDeclaration
{
    __AliceString* ObjectName;
    __AliceString* FieldName;
};

union Union__ParserNodeDeclaration
{
    FunctionHeadDeclaration FieldFunctionHead;
    BodyDeclaration FieldBody;
    FunctionImplDeclaration FieldFunctionImpl;
    VariableAssignDeclaration FieldVariableAssign;
    MemberDeclaration FieldMember;
    ReturnDeclaration FieldReturn;
    Expression FieldExpression;
};

typedef enum
{
    AsFunctionHead = 1, 
    AsBody = 2,
    AsFunctionImpl = 3,
    AsVariableAssign = 4,
    AsVariableReassign = 5,
    AsExpression = 6,
    AsMember = 7,
    AsReturn = 8
} ParserNodeType;

struct Struct__ParserNode {
    ParserNodeType Type;
    ParserNodeDeclaration Declaration;
};

// Parser Tools
typedef enum Enum__ParserStateContextFlag 
{
    NullFlag
} ParserStateContextFlag;

struct Struct__ParserStateContext 
{
    __AliceString* Name;
    AliceToken* AsToken;
    ParserStateContextFlag AsFlag;
};

struct Struct__ParserState 
{
    ParserStateContext* Context;
    __AliceVector* InputTokens;
    ExpressionReader* Reader;
    Usize Pointer;
    AliceToken* Actual;
    __AliceVector* GeneratedNodes;
    __AliceStack* TokenStack;
};

ParserState* NewParserState(__AliceVector* Tokens);
ParserNode* NewParserNode(ParserNodeType Type, ParserNodeDeclaration Declaration);
ParserNode* Parsenize(ParserState* AliceState, AliceManagementObject* ManagementObject);
ParserNode* ParsenizeMember(ParserState* AliceState);
__AliceType* ParsenizeAliceType(ParserState* AliceState);
__AliceType* ParsenizeAliceTypeMember(ParserState* AliceState);
Expression* ParsenizeExpression(ParserState* AliceState);
ParserNode*(*FunctionByType(AliceTokenType T))(ParserState*);
ParserNode* ParsenizeBody(ParserState* AliceState);
ParserNode* ParsenizeDeclaration(ParserState* AliceState);

#endif