#include "Headers/Parser.h"
#include "External/Libraries.h"
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
#define APeek(E) ((ParserPeek(AliceState, 1) -> Type) == (E))
#define Is(E) ((ParserActual(&LocalState) -> Type) == (E))

Bool ParserExpect(ParserState* AliceState, Alice_TokenType Expect, int Offset)
{
    Usize Pos = AliceState -> Pointer + Offset;
    Alice_Token* t = Get(AliceState -> Tokens, Pos);
    return t -> Type == Expect ? True : False;
}

void Test(ParserState* AliceState)
{
    printf("Alice!");
}

void Parsenize(Alice_BigGirl* BigGirl, Vector* Tokens) 
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
        if Is(Tk_Alice)
        { 
            printf("Alice!\n");
            ParserNext(&LocalState, 1);
        }
        else 
        {
            ParserNext(&LocalState, 1);
        }
    }
}
