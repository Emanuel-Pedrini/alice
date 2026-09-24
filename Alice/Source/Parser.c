#include "Headers/Parser.h"
#include "External/Libraries.h"
void ParserNext(ParserState* AliceState, int Offset) 
{
    AliceState -> Pointer += 1 + Offset;
    Alice_Token* t = Get(AliceState -> Tokens, AliceState -> Pointer);
    AliceState -> Actual = t;
}

void ParserSet(ParserState* AliceState, Usize SetValue) 
{
    AliceState -> Pointer = SetValue;
    Alice_Token* t = Get(AliceState -> Tokens, AliceState -> Pointer);
    AliceState -> Actual = t;
}

Alice_Token* ParserActual(ParserState* AliceState) 
{
    return AliceState -> Actual;
}

Alice_Token* ParserPeek(ParserState* AliceState, int Offset)
{
    Usize Pos = AliceState -> Pointer + Offset;
    Alice_Token* t = Get(AliceState -> Tokens, Pos);
    return t;
}

Bool ParserExpect(ParserState* AliceState, Alice_TokenType Expect, int Offset)
{
    Usize Pos = AliceState -> Pointer + Offset;
    Alice_Token* t = Get(AliceState -> Tokens, Pos);
    return t -> Type == Expect ? True : False;
}

void Parsenize(Alice_BigGirl* BigGirl, Vector* Tokens) 
{
    ParserState LocalState;
    LocalState.Pointer = 0;
    LocalState.Tokens = Tokens;
    LocalState.Actual = NULL;

    Alice_Token* y;
    y -> Type = Tk_Illegal;
    y -> Length = 0;
    y -> Start = 0;

    for (int x = 0 ; y -> Type != Tk_EndOfCode ; x++)
    {
        y = Get(Tokens, x);
    }
}