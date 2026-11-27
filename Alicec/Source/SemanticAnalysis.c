#include "Headers/SemanticAnalysis.h"



SemanticAnalyserState* NewSemanticAnalyserState(ParserNode* AST)
{
    SemanticAnalyserState* AnalyserS = __AlAllocate(sizeof(SemanticAnalyserState));
    AnalyserS -> ScopeStack = __NewAlStack(sizeof(__AliceDictionary*));
    AnalyserS -> AST = AST;
    AnalyserS -> ReturnStack = __NewAlStack(sizeof(__AliceType*));
    AnalyserS -> Context = NULL;
    return AnalyserS;
}

SemanticAnalysisResult* CollectGlobalTypesSemanticAnalyserState(SemanticAnalyserState* AliceState, BodyDeclaration* N)
{
}

SemanticAnalysisResult* CollectGlobalFunctionsSemanticAnalyserState(SemanticAnalyserState* AliceState, BodyDeclaration* N)
{
    for (Usize Interator = 0; Interator < N -> BodyStatements -> VSize; Interator++)
    {
        ParserNode* Limb = ((ParserNode*)__GetAlVector(N -> BodyStatements, Interator));
        if (Limb -> Type == AsFunctionHead) 
        {
            FunctionHeadDeclaration Declr = Limb -> Declaration.FieldFunctionHead;
            __AliceString* Name = Declr.Name;
            __AliceType* Type = Declr.ReturnType;
            __AliceDictionary* D = (*(__AliceDictionary**)__GetAlVector(AliceState -> ScopeStack -> Elements, 0));
            __NewElementAlDictionary(D, Name, NewScopeObject(Type, NULL));
        }
        else if (Limb -> Type == AsFunctionImpl) 
        {
            FunctionHeadDeclaration* Declr = Limb->Declaration.FieldFunctionImpl.Head;
            __AliceString* Name = Declr -> Name;
            __AliceType* Type = Declr -> ReturnType;
            __AliceDictionary* D = (*(__AliceDictionary**)__GetAlVector(AliceState -> ScopeStack -> Elements, 0));
            __NewElementAlDictionary(D, Name, NewScopeObject(Type, NULL));
        }
    }
}

ScopeContext* NewScopeContext(Bool Exists, ScopeObject* Object)
{
    ScopeContext* TemporaryBuffer = __AlAllocate(sizeof(ScopeContext));
    TemporaryBuffer -> Exists = Exists;
    TemporaryBuffer -> Object = Object;
    return TemporaryBuffer;
}

ScopeObject* NewScopeObject(__AliceType* Type, LocalSymbol* Symbol)
{
    ScopeObject* TemporaryBuffer = __AlAllocate(sizeof(ScopeObject));
    TemporaryBuffer -> Type = Type;
    TemporaryBuffer -> Symbol = Symbol;
    return TemporaryBuffer;
}

static __Void RaiseErrorSemanticAnalyserState(SemanticAnalyserState* AliceState)
{

}

ScopeContext* LookupSemanticAnalyserState(SemanticAnalyserState* AliceState, __AliceString* ScopeObjectName)
{
    Usize Interator = AliceState -> ScopeStack -> Elements -> VSize;
    while (Interator > 0) 
    {
        __AliceDictionary* D = (*(__AliceDictionary**)__DeepLookAlStack(AliceState -> ScopeStack, Interator));
        if (__AlDictionaryContains(D, ScopeObjectName)) return NewScopeContext(True, __GetAlDictionary(D, ScopeObjectName));
        Interator--;
    }
    return NewScopeContext(False, NULL);
}

__Void SearchAllSemanticAnalyserState(SemanticAnalyserState* AliceState)
{

}

__Void StackScopeDictionary(SemanticAnalyserState* AliceState)
{
    __AliceDictionary* NewScope = __NewAlDictionary(sizeof(ScopeObject));
    __PushAlStack(AliceState -> ScopeStack, &NewScope);
}

__Void UnstackScopeDictionary(SemanticAnalyserState* AliceState)
{
    __PopAlStack(AliceState -> ScopeStack);
}

__Void StackScopeObject(SemanticAnalyserState* AliceState, __AliceString* Name, ScopeObject* Object)
{
    __AliceDictionary* D = (*(__AliceDictionary**)__LookAlStack(AliceState -> ScopeStack));
    __NewElementAlDictionary(D, Name, Object);
}


__Void StackReturnType(SemanticAnalyserState* AliceState, __AliceType* Type)
{
    __PushAlStack(AliceState -> ReturnStack, &Type);
}

__Void UnstackReturnType(SemanticAnalyserState* AliceState)
{
    __PopAlStack(AliceState -> ReturnStack);
}

LocalSymbol* NewLocalSymbol(__AliceString* Name, __AliceType* Type)
{
    LocalSymbol* Symbol = __AlAllocate(sizeof(LocalSymbol));
    Symbol -> Name = Name;
    Symbol -> Type = Type;
    Symbol -> Slot = __NoSymbol;
    return Symbol;
}

SemanticAnalysisResultRef NewSemanticAnalysisResult(SemanticAnalyserState* AliceState)
{
    SemanticAnalysisResultRef SemaAnalysis = __AlAllocate(sizeof(SemanticAnalysisResult));
    SemaAnalysis -> Ok = True;
    return SemaAnalysis;
}

SemanticAnalysisResultRef AnalyseSemanticAnalyserState(SemanticAnalyserState* AliceState)
{
    StackScopeDictionary(AliceState);
    CollectGlobalFunctionsSemanticAnalyserState(AliceState, &(AliceState -> AST -> Declaration.FieldBody));
    AnalyseNode(AliceState, AliceState -> AST);
    UnstackScopeDictionary(AliceState);
    return NewSemanticAnalysisResult(AliceState);
}

static __Void AnalyseStatements(SemanticAnalyserState* AliceState, BodyDeclaration* N)
{
    for (Usize Interator = 0; Interator < N -> BodyStatements -> VSize; Interator++)
    {
        ParserNode* Limb = ((ParserNode*)__GetAlVector(N -> BodyStatements, Interator));
        AnalyseNode(AliceState, Limb);
    }
}
static __Void AnalyseBody(SemanticAnalyserState* AliceState, BodyDeclaration* N)
{
    StackScopeDictionary(AliceState);
    AnalyseStatements(AliceState, N);
    UnstackScopeDictionary(AliceState);
}

static __Void AnalyseFunction(SemanticAnalyserState* AliceState, FunctionImplDeclaration* N)
{
    BodyDeclaration* Body = N -> Body;
    __AliceVector* Args = N -> Head -> Arguments -> Members;
    __AliceVector* Flags = N -> Head -> Flags;
    __AliceString* Name = N -> Head -> Name;
    __AliceType* Type = N -> Head -> ReturnType;

    StackScopeDictionary(AliceState);
    StackReturnType(AliceState, Type);

    for (Usize Interator = 0; Interator < Args -> VSize; Interator++)
    {
        MemberDeclaration* Member = ((MemberDeclaration*)__GetAlVector(Args, Interator));
        AnalyseMember(AliceState, Member);
    }

    AnalyseStatements(AliceState, Body);
    UnstackReturnType(AliceState);
    UnstackScopeDictionary(AliceState);
}

static __Void AnalyseReturn(SemanticAnalyserState* AliceState, ReturnDeclaration* N)
{
    __AliceType* ExpectedType = (*(__AliceType**)__LookAlStack(AliceState -> ReturnStack));
    if (N -> Value == NULL)
    {
        if (ExpectedType != NULL) RaiseErrorSemanticAnalyserState(AliceState);
        return;
    }
    __AliceType* ExprType = AnalyseExpression(AliceState, N -> Value);
    N -> Value -> ResolvedType = ExprType;
    if (!(__CompareAliceType(ExprType, ExpectedType)))
    {
        RaiseErrorSemanticAnalyserState(AliceState);
        return;
    }
    return;
}

static __AliceType* AnalyseExpression(SemanticAnalyserState* AliceState, Expression* N)
{
    switch (N -> Type)
    {
        case LITERAL:
        {
            switch (N -> Value.Literal->Type)
            {
                case ALICETK_NUMBER:
                    N -> ResolvedType = __StandardIntegerType;
                    return N -> ResolvedType;
                case ALICETK_DECIMAL:
                {
                    N -> ResolvedType = __StandardFloatType;
                    return N -> ResolvedType;
                }
                case ALICETK_IDENTIFIER:
                {
                    ScopeContext* Cont = LookupSemanticAnalyserState(AliceState, __AlStringFrom(N -> Value.Literal -> Literal));
                    if (Cont -> Exists)
                    {
                        N -> Symbol = Cont -> Object -> Symbol;
                        N -> ResolvedType = Cont -> Object -> Type;
                        return N -> ResolvedType;
                    }
                    RaiseErrorSemanticAnalyserState(AliceState);
                    N -> ResolvedType = &GlobalNull;
                    return N -> ResolvedType;
                }
                default:
                {
                    RaiseErrorSemanticAnalyserState(AliceState);
                    return &GlobalNull;
                }
            }
        }
        case UNARY:
        {
            AliceTokenType OprType = N -> Value.Unary -> Operator -> Type;
            Bool BooleanOperand = __IsBooleanOperator(OprType);
            switch (OprType)
            {
                case ALICETK_MINUS:
                    break;
                case ALICETK_PLUS:
                    break;
                case ALICETK_EXPLICITCAST:
                    break;
                case ALICETK_COWARDCAST:
                    break;
                default:
                    break;
            }
            __AliceType* UnaryOpr = AnalyseExpression(AliceState, N -> Value.Unary -> Operand);
            if (BooleanOperand) UnaryOpr = &GlobalBoolean;
            N -> ResolvedType = UnaryOpr;
            return UnaryOpr;
        }
        case BINARY:
        {
            Bool BooleanOperand = __IsBooleanOperator(N -> Value.Binary -> Operator -> Type);
            __AliceType* LeftType = AnalyseExpression(AliceState, N -> Value.Binary -> Left);
            __AliceType* RightType = AnalyseExpression(AliceState, N -> Value.Binary -> Right);
            __AliceType* InferedType = LeftType;
            if (BooleanOperand) InferedType = &GlobalBoolean;
            else if (!__CompareAliceType(LeftType, RightType)) RaiseErrorSemanticAnalyserState(AliceState);
            N -> ResolvedType = InferedType;
            return InferedType;
        }
        default:
            RaiseErrorSemanticAnalyserState(AliceState);
            return &GlobalNull;
    }
}

static __Void AnalyseVariableAssign(SemanticAnalyserState* AliceState, VariableAssignDeclaration* N)
{
    MemberDeclaration* Member = N -> Member;
    if (N -> Value != NULL)
    { 
        __AliceType* Type = AnalyseExpression(AliceState, N -> Value);
        if (N -> Member -> Symbol == NULL) NewLocalSymbol(__AlStringFrom(Type -> Literal), Type);
    }
    AnalyseMember(AliceState, N -> Member);
    return;
}

static __Void AnalyseMember(SemanticAnalyserState* AliceState, MemberDeclaration* N)
{
    LocalSymbol* Sym = NewLocalSymbol(N -> Name, N -> Type);
    N -> Symbol = Sym;
    StackScopeObject(AliceState, N -> Name, NewScopeObject(N -> Type, Sym));
}

static __Void AnalyseNode(SemanticAnalyserState* AliceState, ParserNode* N)
{
    switch (N -> Type)
    {
        case AsBody: 
        {
            AnalyseBody(AliceState, &(N -> Declaration.FieldBody));
            break;
        }
        case AsFunctionImpl:
        {
            AnalyseFunction(AliceState, &(N -> Declaration.FieldFunctionImpl)); 
            break;
        }
        case AsReturn:
        {
            AnalyseReturn(AliceState, &(N->Declaration.FieldReturn));
            break;
        }
        case AsExpression:
        {
            AnalyseExpression(AliceState, &(N->Declaration.FieldExpression));
            break;
        }
        case AsVariableAssign:
        {
            AnalyseVariableAssign(AliceState, &(N->Declaration.FieldVariableAssign));
            break;
        }
        case AsMember:
        {
            AnalyseMember(AliceState, &(N -> Declaration.FieldMember));
            break;
        }
    }
}

Bool GetSemanticAnalysisWasSucessfull(SemanticAnalysisResultRef Result)
{
    return (Result != NULL && Result -> Ok);
}