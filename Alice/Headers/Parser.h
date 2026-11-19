#ifndef _ParserH_
#define _ParserH_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/PrattParsing.h"
#include "Headers/Lexer.h"
#include "Headers/AliceTypes.h"

typedef struct Struct__ParserState ParserState;
typedef struct Struct__ParserNode ParserNode;
typedef enum Enum__ParserNodeType ParserNodeType;
typedef enum Enum__FlagType FlagType;

typedef struct Struct__FunctionDeclaration FunctionDeclaration;
typedef struct Struct__VariableDeclaration VariableDeclaration;
typedef struct Struct__MemberDeclaration MemberDeclaration;
typedef struct Struct__FlagDeclaration FlagDeclaration;
typedef struct Struct__MethodCallDeclaration MethodCallDeclaration;
typedef struct Struct__FunctionCallDeclaration FunctionCallDeclaration;
typedef struct Struct__CallArgumentsDeclaration CallArgumentsDeclaration;
typedef struct Struct__MemberGroupDeclaration MemberGroupDeclaration;
typedef struct Struct__FieldAcessDeclaration FieldAcessDeclaration;

typedef union Union__ParserNodeDeclaration ParserNodeDeclaration;

enum Enum__ParserNodeType 
{
    NodeFunction
};

struct Struct__MemberDeclaration // A Common type of declaration
{
    __AliceString* Name;
    __AliceType* Type;
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


struct Struct__FunctionDeclaration
{
    __AliceString* Name;
    __AliceType* ReturnType;
    __AliceVector* Flags; // Vector<FlagDeclaration>*
    MemberGroupDeclaration* Arguments; // Vector<MemberDeclaration>*
};

struct Struct__FunctionCallDeclaration
{
    __AliceString* FunctionName;
    CallArgumentsDeclaration* CallArguments;
};

struct Struct__VariableDeclaration
{
    MemberDeclaration* Member;
    __AliceVector* Flags; // Vector<FlagDeclaration>*
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
    FunctionDeclaration FieldFunction;
    VariableDeclaration FieldVariable;
    Expression FieldExpression;
};


struct Struct__ParserNode {
    ParserNodeType Type;
    ParserNodeDeclaration Declaration;
};

// Parser Tools
struct Struct__ParserState 
{
    __AliceVector* InputTokens;
    Usize Pointer;
    AliceToken* Actual;
    __AliceVector* GeneratedNodes;
    __AliceStack* TokenStack;
};
ParserState* NewParserState(__AliceVector* Tokens);

__AliceVector* Parsenize(ParserState* AliceState, AliceManagementObject* ManagementObject);
ParserNodeDeclaration*(*FunctionByType(AliceTokenType T))(ParserState*);

#endif