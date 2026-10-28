#include "Headers/Parser.h"
#include "External/Libraries.h"

#define NODE(N) ParserNode* N = malloc(sizeof(ParserNode));
#define EXPC(T) (ParserExpect(AliceState, T, 0))
#define CONSUME(T) if (ParserExpect(AliceState, T, 0)) ParserNext(AliceState, 0);
#define SAVE(V) Alice_Token* V = ParserActual(AliceState);
#define SNEXT(V) Alice_Token* V = ParserActual(AliceState);ParserNext(AliceState, 0);
#define NEXT ParserNext(AliceState, 1);

void ParserSet(ParserState* AliceState, Usize SetValue) 
{
    AliceState -> Pointer = SetValue;
}

void ParserNext(ParserState* AliceState, int Offset) 
{
    ParserSet(AliceState, AliceState -> Pointer + (Offset));
}

Alice_Token* ParserActual(ParserState* AliceState) 
{
    return Get(AliceState -> Tokens, AliceState -> Pointer);
}

Alice_Token* ParserPeek(ParserState* AliceState, int Offset)
{
    Usize Pos = AliceState -> Pointer + Offset;
    Alice_Token* t = malloc(sizeof(Alice_Token));
    t = Get(AliceState -> Tokens, Pos);
    return t;
}

Bool ParserExpect(ParserState* AliceState, Alice_TokenType Expect, int Offset)
{
    Usize Pos = AliceState -> Pointer + Offset;
    Alice_Token* t = Get(AliceState -> Tokens, Pos);
    return t -> Type == Expect ? True : False;
}

ParserNode* ParseType(ParserState* AliceState)
{

}

ParserNode* ParseIndentifier(ParserState* AliceState)
{
    NODE(N)
    Alice_Token* NameIndentifier = ParserActual(AliceState);
    ParserNext(AliceState, 1);
    if EXPC(Tk_Colon)
    {
        /* Probably the creation of a variable! */
        NEXT
        
        // Now we expect the variable type.
        // Soon will try to add simple type inference.

        if EXPC(Tk_Indentifier)
        {
            SNEXT(TypeIdentifier)
        }
        else if EXPC(Tk_Equal)
        {
            /* A value attribution! */
            NEXT
        }
        else 
        {
            /* Only variable declaration! */
            NEXT
        }
    }
    else if EXPC(Tk_Equal)
    {

    }
}

ParserNode* ParseOperation(ParserState* AliceState)
{

}

ParserNode* ParserRead(ParserState* AliceState)
{
    Alice_Token* A = ParserActual(AliceState);
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

#define NextIs(E) ((ParserPeek(&LocalState, 1) -> Type) == (E))
#define Is(E) ((ParserActual(&LocalState) -> Type) == (E))
#define Alice (Is(Tk_Alice) && NextIs(Tk_Comma))

Vector* Parsenize(Alice_BigGirl* BigGirl, Vector* Tokens) 
{
    ParserState LocalState;
    LocalState.Pointer = 0;
    LocalState.Tokens = Tokens;
    LocalState.Actual = NULL;

    VECTOR(NodeVector, ParserNode)

    Alice_Token* Token = malloc(sizeof(Alice_Token));
    Token -> Type = Tk_Illegal;
    Token -> Length = 0; 
    Token -> Start = 0;
    while (1)
    {
        if Is(Tk_EndOfCode) break;
        else
        {
            ParserRead(&LocalState);
        }
    }
    return NULL;
}
