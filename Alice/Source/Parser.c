#include "Headers/Parser.h"
#include "External/Libraries.h"

#define NODE(N) ParserNode* N = malloc(sizeof(ParserNode));
#define EXPC(T) (ParserExpect(AliceState, T, 0))
#define CONSUME(T) if (ParserExpect(AliceState, T, 0)) ParserNext(AliceState, 1);
#define SAVE(V) AliceToken* V = ParserActual(AliceState);
#define SAVE_NEXT(V) AliceToken* V = ParserActual(AliceState);ParserNext(AliceState, 1);
#define NEXT ParserNext(AliceState, 1);

#define NEXT_IS(E) ((ParserPeek(&LocalState, 1) -> Type) == (E))
#define IS(E) ((ParserActual(&LocalState) -> Type) == (E))
#define Alice (IS(Tk_Alice) && NEXT_IS(Tk_Comma))

void ParserSet(ParserState* AliceState, Usize SetValue) 
{
    AliceState -> Pointer = SetValue;
}

void ParserNext(ParserState* AliceState, int Offset) 
{
    ParserSet(AliceState, AliceState -> Pointer + (Offset));
}

AliceToken* ParserActual(ParserState* AliceState) 
{
    return GetByIndexVector(AliceState -> Tokens, AliceState -> Pointer);
}

AliceToken* ParserPeek(ParserState* AliceState, int Offset)
{
    Usize Pos = AliceState -> Pointer + Offset;
    AliceToken* t = malloc(sizeof(AliceToken));
    t = GetByIndexVector(AliceState -> Tokens, Pos);
    return t;
}

Bool ParserExpect(ParserState* AliceState, AliceTokenType Expect, int Offset)
{
    Usize Pos = AliceState -> Pointer + Offset;
    AliceToken* t = GetByIndexVector(AliceState -> Tokens, Pos);
    return t -> Type == Expect ? True : False;
}

ParserNode* ParseType(ParserState* AliceState)
{

}

ParserNode* ParsePrefix(ParserState* AliceState)
{
    NODE(N)
    return N;
}


ParserNode* ParseIndentifier(ParserState* AliceState)
{
    NODE(N)
    SAVE_NEXT(NameIdentifier)
    if EXPC(Tk_Colon)
    {
        NEXT
        if EXPC(Tk_Indentifier)
        {
            SAVE_NEXT(TypeIdentifier)
        }
        else if EXPC(Tk_Equal)
        {
            NEXT
        }
        else 
        {
            NEXT
        }
    }
    else if EXPC(Tk_Equal)
    {

    }

    else if EXPC(Tk_Point)
    {

    }
}

ParserNode* ParseOperation(ParserState* AliceState)
{

}

ParserNode* ParserRead(ParserState* AliceState)
{
    AliceToken* A = ParserActual(AliceState);
    Usize L = 0;
    
    switch (A -> Type)
    {
        case Tk_Indentifier:
            ParseIndentifier(AliceState);
            break;
        default:
            L = 1;
            break;
    }
    ParserNext(AliceState, L);
}

Vector* Parsenize(AliceManagementObject* BigGirl, Vector* Tokens) 
{
    ParserState LocalState;
    LocalState.Pointer = 0;
    LocalState.Tokens = Tokens;
    LocalState.Actual = NULL;

    VECTOR(NodeVector, ParserNode)

    AliceToken* Token = malloc(sizeof(AliceToken));
    Token -> Type = Tk_Illegal;
    Token -> Length = 0; 
    Token -> Start = 0;
    while (1)
    {
        if IS(Tk_EndOfCode) break;
        else
        {
            ParserRead(&LocalState);
        }
    }
    return NULL;
}
