#ifndef _FOWARD_INCLUDE_
#define _FOWARD_INCLUDE_

typedef void __Void;
typedef void* __VoidPointer;

typedef unsigned short int Bool;
typedef unsigned long long int Usize;
typedef long long int Dsize;
typedef uint8_t Placeholder;
typedef const char* CString;

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

typedef union Union__AliceCodeContent AliceCodeContent;
typedef struct Struct__AliceCodeContext AliceCodeContext;
typedef struct Struct__AliceCodeLine AliceCodeLine;
typedef struct Struct__AliceCodeFormat AliceCodeFormat;
typedef struct Struct__AliceCodeBlock AliceCodeBlock;
typedef struct Struct__AliceCodeWritter AliceCodeWritter;
typedef struct Struct__AliceCompilerResult AliceCompilerResult;

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

typedef struct Struct__AliceIRValue AliceIRValue;
typedef struct Struct__AliceInstructionType AliceInstructionType;
typedef struct Struct__AliceIRInstruction AliceIRInstruction;
typedef struct Struct__AliceIRBlock AliceIRBlock;
typedef struct Struct__AliceIRFunction AliceIRFunction;
typedef struct Struct__AliceIRModule AliceIRModule;
typedef struct Struct__AliceIRBuilder AliceIRBuilder;
typedef struct Struct__AliceIRContext AliceIRContext;
typedef struct Struct__AliceIRSlot AliceIRSlot;

struct Struct__LocalSymbol
{
    __AliceString* Name;
    __AliceType* Type;
    Usize Slot;
};

typedef struct Struct__AliceCompilationAttributes AliceCompilationAttributes;
typedef struct Struct__AliceCompilationAttributesReader AliceCompilationAttributesReader; 
#endif