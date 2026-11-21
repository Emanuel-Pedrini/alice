#ifndef _FOWARD_INCLUDE_
#define _FOWARD_INCLUDE_

typedef void __Void;
typedef void* __VoidPointer;

typedef unsigned short int Bool;
typedef unsigned long long int Usize;
typedef long long int Dsize;
typedef uint8_t Placeholder;
typedef const char* __CString;

typedef struct Struct__AliceString  __AliceString;
typedef struct Struct__AliceVector  __AliceVector;
typedef struct Struct__AliceStack   __AliceStack;
typedef struct Struct__AliceType    __AliceType;
typedef struct Struct__Expression   Expression;

typedef union Union__AliceTypeInformation __AliceTypeInformation;
typedef struct Struct__AliceTypeComplexInformation __AliceTypeComplexInformation;
typedef struct Struct__AliceTypeGenericInformation __AliceTypeGenericInformation;
typedef struct Struct__AliceTypePointerInformation __AliceTypePointerInformation;

typedef struct Struct__SemanticAnalyserState SemanticAnalyserState;
typedef struct Struct__SemanticAnalysis SemanticAnalysis;
typedef struct Struct__ScopeContext ScopeContext;
typedef struct Struct__ScopeObject ScopeObject;
typedef struct Struct__LocalSymbol LocalSymbol;

// Some i'll say this is a level 999 witchery and i need to be killed but you can't stop me >:)
#define __Declaration(Name) typedef struct Struct__##Name##Declaration Name##Declaration;
__Declaration(FunctionHead)
__Declaration(Body)
__Declaration(FunctionImpl)
__Declaration(VariableAssign)
__Declaration(Member)
__Declaration(Flag)
__Declaration(MethodCall)
__Declaration(FunctionCall)
__Declaration(CallArguments)
__Declaration(MemberGroup)
__Declaration(FieldAcess)
__Declaration(VariableReassign)
__Declaration(Return)
typedef struct Struct__ParserState ParserState;
typedef struct Struct__ParserNode ParserNode;
typedef struct Struct__ParserStateContext ParserStateContext;

typedef union Union__ParserNodeDeclaration ParserNodeDeclaration;

#endif