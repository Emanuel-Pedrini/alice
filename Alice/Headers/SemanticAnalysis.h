#ifndef _SemanticAnalysisH_
#define _SemanticAnalysisH_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceTypes.h"
#include "Headers/Parser.h"
#include "Headers/ForwardInclude.h"

struct Struct__LocalSymbol
{
    __AliceString* Name;
    __AliceType* Type;
    Usize Slot;
};

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

struct SemanticAnalysis
{
    Bool FullResolution;
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
SemanticAnalysis* AnalyseSemanticAnalyserState(SemanticAnalyserState* AliceState);
SemanticAnalysis* CollectGlobalTypesSemanticAnalyserState(SemanticAnalyserState* AliceState);
SemanticAnalysis* CollectGlobalFunctionsSemanticAnalyserState(SemanticAnalyserState* AliceState);
ScopeContext* LookupSemanticAnalyserState(SemanticAnalyserState* AliceState, __AliceString* ScopeObjectName);
static __Void AnalyseNode(SemanticAnalyserState* AliceState, ParserNode* N);
static __AliceType* AnalyseExpression(SemanticAnalyserState* AliceState, Expression* N);
#endif