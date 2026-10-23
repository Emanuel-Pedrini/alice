#include "Headers/Parser.h"
#include "External/Libraries.h"

#define Node(N) ParserNode* N = malloc(sizeof(ParserNode));

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

ParserNode* ParserRead(ParserState* AliceState)
{
    Node(N)
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

    Alice_Token* Token = malloc(sizeof(Alice_Token));
    Token -> Type = Tk_Illegal;
    Token -> Length = 0; 
    Token -> Start = 0;
    while (1)
    {
        if Is(Tk_EndOfCode) break;
        if Alice
        { 
            ParserNext(&LocalState, 2);
            printf("> %s\n", ParserActual(&LocalState) -> Literal);
        }
        else 
        {
            ParserNext(&LocalState, 1);
        }
    }
    return NULL;
}
