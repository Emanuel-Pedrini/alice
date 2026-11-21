#ifndef _SemanticAnalysisH_
#define _SemanticAnalysisH_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceTypes.h"
#include "Headers/Parser.h"
#include "Headers/ForwardInclude.h"

typedef struct Struct__SemanticAnalyserState SemanticAnalyserState;
typedef struct Struct__SemanticAnalysis SemanticAnalysis;
typedef struct Struct__ScopeContext ScopeContext;
typedef struct Struct__ScopeObject ScopeObject;

struct Struct__ScopeObject
{
    __AliceType* Type;
};

struct Struct__ScopeContext
{
    Bool Exists;
    ScopeObject* Object;
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
SemanticAnalysis* AnalyseSemanticAnalyserState(SemanticAnalyserState* AliceState);
SemanticAnalysis* CollectGlobalTypesSemanticAnalyserState(SemanticAnalyserState* AliceState);
SemanticAnalysis* CollectGlobalFunctionsSemanticAnalyserState(SemanticAnalyserState* AliceState);
ScopeContext* LookupSemanticAnalyserState(SemanticAnalyserState* AliceState, __AliceString* ScopeObjectName);
static __Void AnalyseNode(SemanticAnalyserState* AliceState, ParserNode* N);
static __AliceType* AnalyseExpression(SemanticAnalyserState* AliceState, Expression* N);
#endif