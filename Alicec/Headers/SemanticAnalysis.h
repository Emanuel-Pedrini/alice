#ifndef _SemanticAnalysisH_
#define _SemanticAnalysisH_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceTypes.h"
#include "Headers/Parser.h"
#include "Headers/ForwardInclude.h"

#define __NoSymbol ((Usize)-1)
#define __StandardFloatType &Globalf64
#define __StandardIntegerType &Globali32

struct Struct__ScopeObject
{
    __AliceType* Type;
    LocalSymbol* Symbol;
};

struct Struct__ScopeContext
{
    Bool Exists;
    ScopeObject* Object;
    LocalSymbol* Symbol;
};

struct Struct__SemanticAnalysisResult
{
    Bool Ok;
};

struct Struct__SemanticAnalyserState
{
    __AliceStack* ScopeStack;
    ParserNode* AST;
    ParserNode* Context;
    __AliceStack* ReturnStack;
};

SemanticAnalyserState* NewSemanticAnalyserState(ParserNode* AST);
LocalSymbol* NewLocalSymbol(__AliceString* Name, __AliceType* Type);
SemanticAnalysisResult* AnalyseSemanticAnalyserState(SemanticAnalyserState* AliceState);
ScopeObject* NewScopeObject(__AliceType* Type, LocalSymbol* Symbol);

SemanticAnalysisResult* CollectGlobalTypesSemanticAnalyserState(SemanticAnalyserState* AliceState, BodyDeclaration* N);
SemanticAnalysisResult* CollectGlobalFunctionsSemanticAnalyserState(SemanticAnalyserState* AliceState, BodyDeclaration* N);

ScopeContext* LookupSemanticAnalyserState(SemanticAnalyserState* AliceState, __AliceString* ScopeObjectName);
static __Void AnalyseNode(SemanticAnalyserState* AliceState, ParserNode* N);
static __AliceType* AnalyseExpression(SemanticAnalyserState* AliceState, Expression* N);
Bool GetSemanticAnalysisWasSucessfull(SemanticAnalysisResultRef Result);
SemanticAnalysisResultRef NewSemanticAnalysisResult(SemanticAnalyserState* AliceState);
static __Void AnalyseMember(SemanticAnalyserState* AliceState, MemberDeclaration* N);
#endif