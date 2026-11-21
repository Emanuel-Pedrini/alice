#include "Headers/SemanticAnalysis.h"

#define RETURN_ANALYSER_MACRO(D) ____RETURN___TYPE___MACRO___EXPRESSI0N__##D

SemanticAnalyserState* NewSemanticAnalyserState(ParserNode* AST)
{
    SemanticAnalyserState* AnalyserS = __AlAllocate(sizeof(SemanticAnalyserState));
    AnalyserS -> ScopeStack = __NewAlStack(sizeof(__AliceDictionary));
    AnalyserS -> AST = AST;
    AnalyserS -> ReturnStack = __NewAlStack(sizeof(__AliceType));
    AnalyserS -> Context = NULL;
    return AnalyserS;
}

SemanticAnalysis* CollectGlobalTypesSemanticAnalyserState(SemanticAnalyserState* AliceState)
{

}

SemanticAnalysis* CollectGlobalFunctionsSemanticAnalyserState(SemanticAnalyserState* AliceState)
{

}

ScopeContext* NewScopeContext(Bool Exists, ScopeObject* Object)
{
    ScopeContext* TemporaryBuffer = __AlAllocate(sizeof(ScopeContext));
    TemporaryBuffer -> Exists = Exists;
    TemporaryBuffer -> Object = Object;
    return TemporaryBuffer;
}

ScopeObject* NewScopeObject(__AliceType* Type)
{
    ScopeObject* TemporaryBuffer = __AlAllocate(sizeof(ScopeObject));
    TemporaryBuffer -> Type = Type;
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
        Interator--;
        __AliceDictionary* D = ((__AliceDictionary*)__DeepLookAlStack(AliceState -> ScopeStack, Interator));
        if (__AlDictionaryContains(D, ScopeObjectName)) return NewScopeContext(True, __GetAlDictionary(D, ScopeObjectName));
    }
    return NewScopeContext(False, NULL);
}

__Void SearchAllSemanticAnalyserState(SemanticAnalyserState* AliceState)
{

}

__Void StackScopeDictionary(SemanticAnalyserState* AliceState)
{
    __AliceDictionary* NewScope = __NewAlDictionary(sizeof(ScopeObject));
    __PushAlStack(AliceState -> ScopeStack, NewScope);
}

__Void UnstackScopeDictionary(SemanticAnalyserState* AliceState)
{
    __PopAlStack(AliceState -> ScopeStack);
}

__Void StackScopeObject(SemanticAnalyserState* AliceState, __AliceString* Name, ScopeObject* Object)
{
    __AliceDictionary* D = ((__AliceDictionary*)__LookAlStack(AliceState -> ScopeStack));
    __NewElementAlDictionary(D, Name, Object);
}


__Void StackReturnType(SemanticAnalyserState* AliceState, __AliceType* Type)
{
    __PushAlStack(AliceState -> ReturnStack, Type);
}

__Void UnstackReturnType(SemanticAnalyserState* AliceState)
{
    __PopAlStack(AliceState -> ReturnStack);
}


SemanticAnalysis* AnalyseSemanticAnalyserState(SemanticAnalyserState* AliceState)
{
    StackScopeDictionary(AliceState);
    AnalyseNode(AliceState, AliceState -> AST);
    UnstackScopeDictionary(AliceState);
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

    StackScopeObject(AliceState, Name, NewScopeObject(NULL));
    StackScopeDictionary(AliceState);
    StackReturnType(AliceState, Type);

    __ForIn(Args, I, 0)
    {
        MemberDeclaration* Member = ((MemberDeclaration*)__GetAlVector(Args, I));
        StackScopeObject(AliceState, Member -> Name, NewScopeObject(Member -> Type));
    }

    AnalyseStatements(AliceState, Body);
    UnstackReturnType(AliceState);
    UnstackScopeDictionary(AliceState);
}

static __Void AnalyseReturn(SemanticAnalyserState* AliceState, ReturnDeclaration* N)
{
    __AliceType* ExpectedType = ((__AliceType*)__LookAlStack(AliceState -> ReturnStack));
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
                case Tk_Number:
                case Tk_Decimal:
                {
                    __AliceType* Fallback = __NewAlGenericType("i32", AlType_i32, NULL);
                    N -> ResolvedType = Fallback;
                    return Fallback;
                }
                case Tk_Indentifier:
                {
                    ScopeContext* Cont = LookupSemanticAnalyserState(AliceState, __AlStringFrom(N -> Value.Literal -> Literal));
                    if (Cont -> Exists)
                    {
                        __AliceType* Type = Cont -> Object -> Type;
                        N -> ResolvedType = Type;
                        return Type;
                    }
                    RaiseErrorSemanticAnalyserState(AliceState);
                    return __NewAlGenericType("null", AlType_Null, NULL);
                }
                default:
                {
                    RaiseErrorSemanticAnalyserState(AliceState);
                    return __NewAlGenericType("null", AlType_Null, NULL);
                }
            }
        }
        case UNARY:
        {
            Bool BooleanOperand = __IsBooleanOperator(N -> Value.Unary -> Operator -> Type);
            __AliceType* UnaryOpr = AnalyseExpression(AliceState, N -> Value.Unary->Operand);
            if (BooleanOperand) UnaryOpr = __NewAlGenericType("bool", AlType_Boolean, NULL);
            N -> ResolvedType = UnaryOpr;
            return UnaryOpr;
        }
        case BINARY:
        {
            Bool BooleanOperand = __IsBooleanOperator(N -> Value.Binary -> Operator -> Type);
            __AliceType* LeftType = AnalyseExpression(AliceState, N -> Value.Binary -> Left);
            __AliceType* RightType = AnalyseExpression(AliceState, N -> Value.Binary -> Right);
            __AliceType* InferedType;
            if (BooleanOperand) InferedType = __NewAlGenericType("bool", AlType_Boolean, NULL);
            if (__CompareAliceType(LeftType, RightType)) InferedType = LeftType;
            RaiseErrorSemanticAnalyserState(AliceState); 
            N -> ResolvedType = InferedType;
            return LeftType;
        }
        default:
            RaiseErrorSemanticAnalyserState(AliceState);
            __AliceType* Fallback = __NewAlGenericType("null", AlType_Null, NULL);
            N -> ResolvedType = Fallback;
            return Fallback;
    }
}

static __Void AnalyseVariableAssign(SemanticAnalyserState* AliceState, VariableAssignDeclaration* N)
{

}

static __Void AnalyseMember(SemanticAnalyserState* AliceState, MemberDeclaration* N)
{

}

static __Void AnalyseNode(SemanticAnalyserState* AliceState, ParserNode* N)
{
    switch (N -> Type)
    {
        case AsBody: {AnalyseBody(AliceState, &(N -> Declaration.FieldBody));break;}
        case AsFunctionImpl:{AnalyseFunction(AliceState, &(N -> Declaration.FieldFunctionImpl)); break;}
        case AsReturn:{AnalyseReturn(AliceState, &(N->Declaration.FieldReturn));break;}
        case AsExpression:{AnalyseExpression(AliceState, &(N->Declaration.FieldExpression));break;}
        case AsVariableAssign:{AnalyseVariableAssign(AliceState, &(N->Declaration.FieldVariableAssign));break;}
        case AsMember:{AnalyseMember(AliceState, &(N -> Declaration.FieldMember));break;}
    }
}