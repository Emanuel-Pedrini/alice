#include "Headers/Parser.h"
#include "External/Libraries.h"

ParserState* NewParserState(__AliceVector* Tokens)
{
    ParserState* TemporaryBuffer = __AlAllocate(sizeof(ParserState));
    TemporaryBuffer -> Actual = __GetAlVector(Tokens, 0);
    TemporaryBuffer -> Pointer = 0;
    TemporaryBuffer -> InputTokens = Tokens;
    TemporaryBuffer -> GeneratedNodes = __NewAlVector(sizeof(ParserNode));
    TemporaryBuffer -> TokenStack = __NewAlStack(sizeof(AliceToken));
    return TemporaryBuffer;
}

AliceToken* CurrentTokenParserState(ParserState* AliceState)
{
    return ((AliceToken*)__GetAlVector(AliceState -> InputTokens, AliceState -> Pointer));
}

AliceToken* PeekTokenParserState(ParserState* AliceState, int32_t Offset)
{
    int32_t AcessIndex = AliceState -> Pointer + Offset;
    if (AcessIndex < 0 || AcessIndex > AliceState -> InputTokens -> VSize) return NULL; 
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


ParserNodeDeclaration* ParsenizeIndentifier(ParserState* AliceState)
{
    AliceToken* IndentifierToken = ConsumeTokenParserState(AliceState);
    switch (CurrentTokenParserState(AliceState) -> Type)
    {
        case Tk_Colon:
            break;
        case Tk_Equal:
            break;
        default:
            break;
    }
};

ParserNodeDeclaration* ParsenizeDeclaration(ParserState* AliceState)
{
    AliceToken* ActualToken = PeekTokenParserState(AliceState, 0);
}

ParserNodeDeclaration*(*FunctionByType(AliceTokenType T))(ParserState*)
{
    switch (T)
    {
        case Tk_Indentifier: return ParsenizeIndentifier;
        default: 
            return NULL;
    }
}

__AliceVector* Parsenize(ParserState* AliceState, AliceManagementObject* ManagementObject)
{
    return NULL;
    while(!(PeekTokenParserState(AliceState, 0) -> Type == Tk_EndOfCode || PeekTokenParserState(AliceState, 0) == NULL))
    {
        ParsenizeDeclaration(AliceState);
        FowardPointerParserState(AliceState);
    }
}
