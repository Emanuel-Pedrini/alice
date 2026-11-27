#include "Headers/Parser.h"
#include "External/Libraries.h"
#include "External/Libraries.h"

ParserState* NewParserState(__AliceVector* Tokens)
{
    ParserState* TemporaryBuffer = __AlAllocate(sizeof(ParserState));
    TemporaryBuffer -> Actual = __GetAlVector(Tokens, 0);
    TemporaryBuffer -> Pointer = 0;
    TemporaryBuffer -> InputTokens = Tokens;
    TemporaryBuffer -> GeneratedNodes = __NewAlVector(sizeof(ParserNode));
    TemporaryBuffer -> TokenStack = __NewAlStack(sizeof(AliceToken));
    TemporaryBuffer -> Reader = NewExpressionReader();
    TemporaryBuffer -> Reader -> ConstExpression = Tokens;
    TemporaryBuffer -> Reader -> RightDelimiter = ALICETK_ANYTHING;
    return TemporaryBuffer;
}

ParserNode* NewParserNode(ParserNodeType Type, ParserNodeDeclaration Declaration)
{
    ParserNode* TemporaryBuffer = __AlAllocate(sizeof(ParserNode));
    TemporaryBuffer -> Type = Type;
    TemporaryBuffer -> Declaration = Declaration;
    return TemporaryBuffer;
}

AliceToken* CurrentTokenParserState(ParserState* AliceState)
{
    return ((AliceToken*)__GetAlVector(AliceState -> InputTokens, AliceState -> Pointer));
}

AliceToken* PeekTokenParserState(ParserState* AliceState, int32_t Offset)
{
    int32_t AcessIndex = AliceState -> Pointer + Offset;
    if (AcessIndex < 0 || AcessIndex >= AliceState -> InputTokens -> VSize) return NULL; 
    return ((AliceToken*)__GetAlVector(AliceState -> InputTokens, AcessIndex));
}
__Void SetPointerParserState(ParserState* AliceState, Usize NewValue)
{
    if (NewValue > AliceState -> InputTokens -> VSize) return;
    AliceState -> Pointer = NewValue;
    AliceState -> Actual = CurrentTokenParserState(AliceState);
}

__Void FowardPointerParserState(ParserState* AliceState)
{
    SetPointerParserState(AliceState, AliceState -> Pointer + 1);
}

AliceToken* ConsumeTokenParserState(ParserState* AliceState)
{
    AliceToken* T = PeekTokenParserState(AliceState, 0);
    FowardPointerParserState(AliceState);
    return T;
}

__Void RaiseParserError(ParserState* AliceState)
{

}

AliceToken* ExpectTokenParserState(ParserState* AliceState, AliceTokenType Expected)
{
    Bool Result = PeekTokenParserState(AliceState, 0) -> Type == Expected ? True : False;
    if (!Result) {RaiseParserError(AliceState); return NULL;}
    return ConsumeTokenParserState(AliceState);
}

Bool JudgeTokenParserState(ParserState* AliceState, AliceTokenType Expected)
{
    Bool Result = PeekTokenParserState(AliceState, 0) -> Type == Expected ? True : False;
    if (!Result) RaiseParserError(AliceState);
    return Result;
}

ParserNode* ParsenizeMember(ParserState* AliceState)
{
    AliceToken* IdentifierToken = ExpectTokenParserState(AliceState, ALICETK_IDENTIFIER);
    if (IdentifierToken == NULL) return NULL;
    ExpectTokenParserState(AliceState, MEMBER_DELIMITER);
    __AliceType* Type = ParsenizeAliceType(AliceState);
    ParserNodeDeclaration Declaration;
    MemberDeclaration Member = {__AlStringFrom(IdentifierToken -> Literal), Type};
    Declaration.FieldMember = Member;
    return NewParserNode(AsMember, Declaration);
}

__AliceVector* ParsenizeMemberGroup(ParserState* AliceState, AliceTokenType Delimiter)
{
    __AliceVector* MemberGroup = __NewAlVector(sizeof(MemberDeclaration));
    if (PeekTokenParserState(AliceState, 0) -> Type == Delimiter) return MemberGroup;
    while (True)
    {
        ParserNode* Member = ParsenizeMember(AliceState);
        if (Member != NULL)
        {
            __PushAlVector(MemberGroup, &(Member->Declaration.FieldMember));
        }
        if (PeekTokenParserState(AliceState, 0) -> Type == ARGUMENT_DELIMITER) 
        {
            FowardPointerParserState(AliceState);
            continue;
        }
        else if (PeekTokenParserState(AliceState, 0) -> Type == Delimiter) break;
        else 
        {
            RaiseParserError(AliceState);
            FowardPointerParserState(AliceState);
        }
    }
    return MemberGroup;
}

__AliceType* ParsenizeAliceType(ParserState* AliceState)
{
    AliceToken* ActualToken = PeekTokenParserState(AliceState, 0);
    if (ActualToken != NULL) 
    {
        if (ActualToken -> Type == POINTER_DELIMITER)
        {
            FowardPointerParserState(AliceState);
            __AliceType* T = ParsenizeAliceType(AliceState);
            return __NewAliceType("", AlType_Pointer, T);
        }
        else TWO_TOKEN_DECLR(ALICETK_IDENTIFIER, ALICETK_LEFTARROW)
        {
            AliceToken* IdentifierToken = ConsumeTokenParserState(AliceState);
            FowardPointerParserState(AliceState);
            __AliceType* T = ParsenizeAliceType(AliceState);
            __AliceType* G = __NewAliceType(IdentifierToken -> Literal, AlType_Generic, T);
            while (True)
            {
                if (ActualToken -> Type == ARGUMENT_DELIMITER) 
                {
                    FowardPointerParserState(AliceState);
                    AliceState -> Reader -> RightDelimiter = ALICETK_RIGHTARROW;
                    Expression* Expr = ParsenizeExpression(AliceState);
                    G -> Information.Generic.SizeExpression = Expr;
                    G -> Information.Generic.SizeExpressed = True;
                    AliceState -> Reader -> RightDelimiter = ALICETK_ANYTHING;
                }
                else 
                {
                    break;
                }
            }
            ExpectTokenParserState(AliceState, ALICETK_RIGHTARROW);
            return G;
        }
        else if (ActualToken -> Type == ALICETK_IDENTIFIER)
        {
            AliceToken* IdentifierToken = ExpectTokenParserState(AliceState, ALICETK_IDENTIFIER);
            __AliceType* Type = __NativeAlTypeFromString(__AlStringFrom(IdentifierToken -> Literal), False);
            if (Type == NULL)
            {
                Type = __NewAliceType(IdentifierToken -> Literal, AlType_Userdata, NULL);
            }
            return Type;
        }
        else 
        {
            RaiseParserError(AliceState);
            return NULL;
        }
    }
    return NULL;
}

Expression* ParsenizeExpression(ParserState* AliceState)
{
    AliceState -> Reader -> Pointer = AliceState -> Pointer;
    Expression* Expr = ParseExpression(AliceState -> Reader, 0);
    AliceState -> Pointer = AliceState -> Reader -> Pointer;
    return Expr;
}

ParserNode* ParsenizeVariableAssign(ParserState* AliceState)
{
    ParserNode* AssignmentMember = ParsenizeMember(AliceState);
    __AliceVector* AssignmentFlags = __NewAlVector(sizeof(FlagDeclaration));
    Expression* AssignmentValue = NULL;
    ParserNodeDeclaration Declaration;

    if (PeekTokenParserState(AliceState, 0) -> Type == ASSOCIATIVE_DELIMITER)
    {
        FowardPointerParserState(AliceState);
        AssignmentValue = ParsenizeExpression(AliceState);
    }
    else if (PeekTokenParserState(AliceState, 0) -> Type == ALICETK_NEWLINE) 
    {
        AssignmentValue = NULL; 
    }
    else RaiseParserError(AliceState);

    VariableAssignDeclaration Assign = {&(AssignmentMember -> Declaration.FieldMember), AssignmentFlags, AssignmentValue};
    Declaration.FieldVariableAssign = Assign;
    ParserNode* VariableAssign = NewParserNode(AsVariableAssign, Declaration);
    return VariableAssign;
}

ParserNode* ParsenizeVariableReassign(ParserState* AliceState)
{
    
}

ParserNode* ParsenizeIdentifier(ParserState* AliceState)
{
    switch (PeekTokenParserState(AliceState, 1) -> Type)
    {
        case MEMBER_DELIMITER: return ParsenizeVariableAssign(AliceState);
        case ASSOCIATIVE_DELIMITER: return ParsenizeVariableReassign(AliceState);
        default: return NULL;
    }
};

ParserNode* ParsenizeBody(ParserState* AliceState)
{
    __AliceVector* Nodes = __NewAlVector(sizeof(ParserNode));
    while(!(
        PeekTokenParserState(AliceState, 0) -> Type == ALICETK_ENDOFCODE || PeekTokenParserState(AliceState, 0) -> Type == ALICETK_DEDENT))
    {
        ParserNode* NewNode = ParsenizeDeclaration(AliceState);
        if (NewNode != NULL) __PushAlVector(Nodes, NewNode);
    }
    ExpectTokenParserState(AliceState, ALICETK_DEDENT);
    BodyDeclaration* BodyDeclr = __AlAllocate(sizeof(BodyDeclaration));
    BodyDeclr -> BodyStatements = Nodes;
    ParserNodeDeclaration Declaration;
    Declaration.FieldBody = *BodyDeclr;
    return NewParserNode(AsBody, Declaration);
}

ParserNode* ParsenizeConditionalStatement(ParserState* AliceState)
{

}

ParserNode* ParsenizeReturn(ParserState* AliceState)
{
    FowardPointerParserState(AliceState);
    Expression* Expr = ParsenizeExpression(AliceState);
    ReturnDeclaration* Return = __AlAllocate(sizeof(ReturnDeclaration));
    Return -> Value = Expr;
    ParserNodeDeclaration Declaration;
    Declaration.FieldReturn = *Return; 
    return NewParserNode(AsReturn, Declaration);
}

ParserNode* ParsenizeFunction(ParserState* AliceState)
{
    FowardPointerParserState(AliceState);
    __AliceVector* Flags = __NewAlVector(sizeof(FlagDeclaration));

    while (PeekTokenParserState(AliceState, 0) -> Type != ALICETK_IDENTIFIER && PeekTokenParserState(AliceState, 1) -> Type == ALICETK_LEFTPARENTHESIS)
    {
        FowardPointerParserState(AliceState);
    }
    AliceToken* IdentifierToken = ConsumeTokenParserState(AliceState);
    
    ExpectTokenParserState(AliceState, ALICETK_LEFTPARENTHESIS);
    __AliceVector* Arguments = ParsenizeMemberGroup(AliceState, ALICETK_RIGHTPARENTHESIS);
    ExpectTokenParserState(AliceState, ALICETK_RIGHTPARENTHESIS);
    ExpectTokenParserState(AliceState, ALICETK_RIGHTSETTER);

    __AliceType* ReturnType = ParsenizeAliceType(AliceState);
    
    if (PeekTokenParserState(AliceState, 0) -> Type == ALICETK_NEWLINE)
    {
        // It's a prototype?
        FowardPointerParserState(AliceState);
        FunctionHeadDeclaration Prototype; 
    }
    ExpectTokenParserState(AliceState, ALICETK_COLON);
    ExpectTokenParserState(AliceState, ALICETK_NEWLINE);
    ParserNode* Body = ParsenizeBody(AliceState);

    MemberGroupDeclaration* Group = __AlAllocate(sizeof(MemberGroupDeclaration));
    Group -> Members = Arguments;

    FunctionHeadDeclaration* Head = __AlAllocate(sizeof(FunctionHeadDeclaration)); 
    Head -> Name = __AlStringFrom(IdentifierToken -> Literal);
    Head -> ReturnType = ReturnType;
    Head -> Flags = Flags;
    Head -> Arguments = Group;

    FunctionImplDeclaration FuncImpl = {Head, &(Body -> Declaration.FieldBody)};
    ParserNodeDeclaration Declaration;
    Declaration.FieldFunctionImpl = FuncImpl;

    return NewParserNode(AsFunctionImpl, Declaration);
}

ParserNode* ParsenizeDeclaration(ParserState* AliceState)
{
    ParserNode* Node;
    AliceToken* ActualToken = PeekTokenParserState(AliceState, 0);
    switch (ActualToken -> Type)
    {
        case ALICETK_IDENTIFIER: {Node = ParsenizeIdentifier(AliceState); break;}
        case ALICETK_FUNCTION: {Node = ParsenizeFunction(AliceState); break;}
        case ALICETK_RETURN: {Node = ParsenizeReturn(AliceState); break;}
        default:
            FowardPointerParserState(AliceState);
            Node = NULL;
            break;
    }
    return Node;
}

ParserNode*(*FunctionByType(AliceTokenType T))(ParserState*)
{
    switch (T)
    {
        case ALICETK_IDENTIFIER: return ParsenizeIdentifier;
        default: 
            return NULL;
    }
}

ParserNode* ParsenizeParserState(ParserState* AliceState)
{
    ParserNode* Program = ParsenizeBody(AliceState);
    return Program;
}
