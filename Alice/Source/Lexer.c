#include "Headers/CodeObject.h"
#include "Headers/ArrayUtilities.h"
#include "Headers/StringUtilities.h"
#include "Headers/Lexer.h"
#include "External/Libraries.h"

char ActualChar(LexerState* MermaidState) {
    return MermaidState -> Code -> Chars[MermaidState -> Pointer];
}

char Peek(LexerState* MermaidState, int Offset) {
    return MermaidState -> Code -> Chars[MermaidState -> Pointer + Offset];
}

void Next(LexerState* MermaidState, int Offset) {
    MermaidState -> Pointer += Offset;
}

void Skip(LexerState* MermaidState) {
    while (1) 
    {
        if (ActualChar(MermaidState) == '/' && Peek(MermaidState, 1) == '/') 
        {
            while (ActualChar(MermaidState) != '\n' && !IsInvalid(ActualChar(MermaidState))) {
                if (ActualChar(MermaidState) == '\n') 
                {
                    MermaidState -> Line++;
                }
                Next(MermaidState, 1);
            }
        }

        if ((ActualChar(MermaidState) == '/' && Peek(MermaidState, 1) == '*') && !IsInvalid(ActualChar(MermaidState))) 
        {
            while (!(ActualChar(MermaidState) == '*' && Peek(MermaidState, 1) == '/')) {
                if (ActualChar(MermaidState) == '\n') 
                {
                    MermaidState -> Line++;
                }
                Next(MermaidState, 1);
            }
            Next(MermaidState, 2);
        }

        if ((ActualChar(MermaidState) == ' ' || ActualChar(MermaidState) == '\n') && !IsInvalid(ActualChar(MermaidState))) 
        {
            if (ActualChar(MermaidState) == '\n') 
            {
                MermaidState -> Line++;
            }
            Next(MermaidState, 1);
        } 
        else 
        {
            break;
        } 
    }
}

void Set(LexerState* MermaidState, Usize SetValue) 
{
    if (SetValue >= 0) 
    {
        MermaidState -> Pointer = SetValue;
    }
}

Alice_Token* CreateToken(
    Alice_TokenType Type,
    Usize Start,
    Usize Length
) 
{
    Alice_Token* Buffer = malloc(sizeof(Alice_Token));
    Buffer -> Type = Type;
    Buffer -> Start = Start;
    Buffer -> Length = Length;
    return Buffer;
}

Alice_Token* ReadSpecial(LexerState* MermaidState) 
{
    Alice_TokenType Type = Tk_Illegal;
    Usize Start = MermaidState -> Pointer;
    Usize Length = 1;

    switch (ActualChar(MermaidState)) {
        case '@':
            Type = Tk_Adress;
            break;

        case '(':
            Type = Tk_LeftParenthesis;
            break;
        case ')':
            Type = Tk_RightParenthesis;
            break;
        case '{':
            Type = Tk_LeftKeys;
            break;
        case '}':
            Type = Tk_RightKeys;
            break;
        case '[':
            Type = Tk_LeftBrace;
            break;
        case ']':
            Type = Tk_RightBrace;
            break;

        case '%':
            Type = Tk_Percentage;
            break;
        case '$':
            Type = Tk_Money;
            break;

        case '?':
            Type = Tk_Interrogation;
            break;

        case '*':
            Type = Tk_Asterisk;
            break;
            
        case '<':
            switch (Peek(MermaidState, 1))
            {
                case '-':
                    Type = Tk_LeftSetter;
                    Length = 2;
                    break;

                default:
                    Type = Tk_LeftArrow;
                    break;
            }
            break;
        case '>':
            Type = Tk_RightArrow;
            break;

        case '!':
            Type = Tk_Bang;
            break;

        case '^':
            Type = Tk_UpArrow;
            break;

        case '+':
            switch (Peek(MermaidState, 1))
            {
                case '+':
                    Type = Tk_PlusPLus;
                    Length = 2;
                    break;

                default:
                    Type = Tk_Plus;
                    break;
            }
            break;
        case '-':
            switch (Peek(MermaidState, 1))
            {
                case '-':
                    Type = Tk_MinusMinus;
                    Length = 2;
                    break;

                case '>':
                    Type = Tk_RightSetter;
                    Length = 2;
                    break;

                default:
                    Type = Tk_Minus;
                    break;
            }
            break;
        case ':':
            switch (Peek(MermaidState, 1))
            {
                case ':':
                    Type = Tk_DoubleColon;
                    Length = 2;
                    break;

                default:
                    Type = Tk_Colon;
                    break;
            }
            break;

        case '=':
            switch (Peek(MermaidState, 1))
            {
                default:
                    Type = Tk_Equal;
                    break;
            }
            break;

        case '/':
            switch (Peek(MermaidState, 1))
            {
                case '/':
                    Type = Tk_Div;
                    Length = 2;
                    break;

                default:
                    Type = Tk_DivDiv;
                    break;
            }
            break;

        case '\\':
            switch (Peek(MermaidState, 1))
            {
                case '\\':
                    Type = Tk_DoubleBackslash;
                    Length = 2;
                    break;

                default:
                    Type = Tk_Backslash;
                    break;
            }
            break;
        case '&':
            switch (Peek(MermaidState, 1))
            {
                case '&':
                    Type = Tk_DoubleCommercial;
                    Length = 2;
                    break;

                default:
                    Type = Tk_Commercial;
                    break;
            }
            break;
        case '|':
            switch (Peek(MermaidState, 1))
            {
                case '|':
                    Type = Tk_DoublePipe;
                    Length = 2;
                    break;

                default:
                    Type = Tk_Pipe;
                    break;
            }
            break;
        case ',':
            Type = Tk_Comma;
            break;
        default:
            Type = Tk_Illegal;
            break;
    }
    return CreateToken(Type, Start, Length);
}

Alice_TokenType Indentify(const char* Word) 
{
    if (Is(Word, RETURN_KEYWORD)) 
    {
        return Tk_Return;
    }
    else if (Is(Word, CONSTANT_KEYWORD)) {
        return Tk_Define;
    }
    else if (Is(Word, WRITE_KEYWORD)) {
        return Tk_Write;
    }
    else if (Is(Word, END_OF_CODE_KEYWORD)) {
        return Tk_EndOfCode;
    }
    else 
    {
        return Tk_Indentifier;
    }
}

Alice_Token* ReadIndentifier(LexerState* MermaidState) 
{
    int Length = 0;
    Usize Start = MermaidState -> Pointer;
    while (IsIndentifierChar(MermaidState -> Code -> Chars[Start + Length]) && !IsInvalid(ActualChar(MermaidState))) 
    {
        Length++;
    }
    char* Word =  Substring(MermaidState -> Code -> Chars, Start, (Start + Length));
    Alice_TokenType Type = Indentify(Word);
    return CreateToken(Type, Start, Length);
}

Alice_Token* ReadString(LexerState* MermaidState)
{
    Next(MermaidState, 1);
    int Length = 0;
    Usize Start = MermaidState -> Pointer;
    while ((MermaidState -> Code -> Chars[Start + Length]) != STRING_MARKER && !IsInvalid(ActualChar(MermaidState))) 
    {
        Length++;
    }
    char* Word =  Substring(MermaidState -> Code -> Chars, Start, (Start + Length));

    // Temporary
    printf("%s ", Word);
    Next(MermaidState, 1);
    return CreateToken(Tk_String, Start, Length);
}

Alice_Token* ReadNumerical(LexerState* MermaidState) 
{
    int Length = 0;
    Usize Start = MermaidState -> Pointer;
    while (IsValidNumerical(MermaidState -> Code -> Chars[Start + Length]) && !IsInvalid(ActualChar(MermaidState))) 
    {
        Length++;
    }
    return CreateToken(Tk_Integer, Start, Length);
}

Alice_Token* ReadArchiveMarker(LexerState* MermaidState)
{
    Next(MermaidState, 1);
    int Length = 0;
    Usize Start = MermaidState -> Pointer;
    while ((MermaidState -> Code -> Chars[Start + Length]) != ARCHIVE_MARKER && !IsInvalid(ActualChar(MermaidState))) 
    {
        Length++;
    }
    char* Word =  Substring(MermaidState -> Code -> Chars, Start, (Start + Length));
    Next(MermaidState, 1);
    return CreateToken(Tk_ArchiveMarker, Start, Length);
    MermaidState -> Line = 1;
}

void Tokenize(Alice_BigGirl* Girl, Vector* Tokens) 
{
    LexerState LocalState;
    LocalState.Char = '_';
    LocalState.Pointer = 0;
    LocalState.Line = 1;

    LocalState.Code = Girl -> SourceCode;
    for (int x = 0; ActualChar(&LocalState) != '\0';x++) {
        Skip(&LocalState);
        Usize Offset = 1;

        Alice_Token* LocalToken = NULL;

        if (ActualChar(&LocalState) == ARCHIVE_MARKER) 
        {
            LocalToken = ReadArchiveMarker(&LocalState);
        }
        else if (ActualChar(&LocalState) == STRING_MARKER) 
        {
            LocalToken = ReadString(&LocalState);
        }
        else if (!IsIndentifierChar(ActualChar(&LocalState))) 
        {
            LocalToken = ReadSpecial(&LocalState);
        }
        else if (IsNumerical(ActualChar(&LocalState))) 
        {
            LocalToken = ReadNumerical(&LocalState);
        }
        else if (IsIndentifierChar(ActualChar(&LocalState)))
        {
            LocalToken = ReadIndentifier(&LocalState);
        }
        if (LocalToken != NULL) {
            Offset = LocalToken -> Length;
            Add(Tokens, LocalToken);
        }
        Next(&LocalState, Offset);
    }
}
