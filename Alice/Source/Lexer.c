#include "Headers/CodeObject.h"
#include "Headers/ArrayUtilities.h"
#include "Headers/StringUtilities.h"
#include "Headers/Lexer.h"
#include "External/Libraries.h"

char ActualChar(LexerState* AliceState) {
    return AliceState -> Code -> Chars[AliceState -> Pointer];
}

char Peek(LexerState* AliceState, int Offset) {
    return AliceState -> Code -> Chars[AliceState -> Pointer + Offset];
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

void Next(LexerState* AliceState, int Offset) {
    AliceState -> Pointer += Offset;
}

Usize MeasureIndentation(LexerState* AliceState) 
{
    Usize TabCount = 0;
    Usize LexerPointer = AliceState -> Pointer + 1;
    while ((AliceState -> Code -> Chars[LexerPointer] == '\t') || (AliceState -> Code -> Chars[LexerPointer] == ' ')) 
    {
        TabCount++;
        LexerPointer++;
    }
    return TabCount;
}

Alice_Token* HandleIndentation(LexerState* AliceState) {
    AliceState -> Line++;
    Usize Start = AliceState -> Pointer;
    Usize ActualIndentation = MeasureIndentation(AliceState);
    if (ActualIndentation > AliceState -> Indentation) {
        AliceState -> Indentation = ActualIndentation;
        return CreateToken(
            Tk_Indent,
            Start,
            1);
    }
    else if (ActualIndentation < AliceState -> Indentation) {
        AliceState -> Indentation = ActualIndentation;
        return CreateToken(
            Tk_Dedent,
            Start,
            1);
    }
}

void Skip(LexerState* AliceState) {
    while (1) 
    {
        if (ActualChar(AliceState) == '/' && Peek(AliceState, 1) == '/') 
        {
            while (ActualChar(AliceState) != '\n' 
                    && !IsInvalid(ActualChar(AliceState))) {
                if (ActualChar(AliceState) == '\n') 
                {
                    AliceState -> Line++;
                }
                Next(AliceState, 1);
            }
        }

        if ((ActualChar(AliceState) == '/' && Peek(AliceState, 1) == '*') && !IsInvalid(ActualChar(AliceState))) 
        {
            while (!(ActualChar(AliceState) == '*' && Peek(AliceState, 1) == '/')) {
                if (ActualChar(AliceState) == '\n') 
                {
                    AliceState -> Line++;
                }
                Next(AliceState, 1);
            }
            Next(AliceState, 2);
        }

        if ((ActualChar(AliceState) == ' ') && !IsInvalid(ActualChar(AliceState))) 
        {
            Next(AliceState, 1);
        } 
        else 
        {
            break;
        } 
    }
}

void Set(LexerState* AliceState, Usize SetValue) 
{
    if (SetValue >= 0) 
    {
        AliceState -> Pointer = SetValue;
    }
}

Alice_Token* ReadSpecial(LexerState* AliceState) 
{
    Alice_TokenType Type = Tk_Illegal;
    Usize Start = AliceState -> Pointer;
    Usize Length = 1;

    switch (ActualChar(AliceState)) {
        case '\n':
            if (MeasureIndentation(AliceState) > AliceState -> Indentation 
            || MeasureIndentation(AliceState) < AliceState -> Indentation) 
            {
                Type = HandleIndentation(AliceState) -> Type;
            }
            else {
                return NULL;
            }
            break;
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
            switch (Peek(AliceState, 1))
            {
                case '-':
                    Type = Tk_LeftSetter;
                    Length = 2;
                    break;
                case '=':
                    Type = Tk_LessEqual;
                    Length = 2;
                    break;
                default:
                    Type = Tk_LeftArrow;
                    break;
            }
            break;
        case '>':
            switch (Peek(AliceState, 1))
            {
                case '=':
                    Type = Tk_GreaterEqual;
                    Length = 2;
                    break;
                default:
                    Type = Tk_RightArrow;
                    break;
            }
            break;

        case '!':
            switch (Peek(AliceState, 1))
            {
                case '=':
                    Type = Tk_NotEqual;
                    Length = 2;
                    break;
                default:
                    Type = Tk_Bang;
                    break;
            }
            break;

        case '^':
            Type = Tk_UpArrow;
            break;

        case '+':
            Type = Tk_Plus;

            break;
        case '-':
            switch (Peek(AliceState, 1))
            {
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
            switch (Peek(AliceState, 1))
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
            switch (Peek(AliceState, 1))
            {
                case '=':
                    Type = Tk_EqualEqual;
                    Length = 2;
                    break;

                default:
                    Type = Tk_Equal;
                    break;
            }
            break;

        case '/':
            Type = Tk_Div;
            break;

        case '\\':
            Type = Tk_Backslash;
            break;
        case '&':
            Type = Tk_Commercial;
            break;
        case '|':
            switch (Peek(AliceState, 1))
            {
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
    if (IsCaseEqual(Word, RETURN_KEYWORD)) 
    {
        return Tk_Return;
    }
    else if (IsCaseEqual(Word, CONSTANT_KEYWORD)) {
        return Tk_Constant;
    }
    else if (IsCaseEqual(Word, VARIABLE_KEYWORD)) {
        return Tk_Variable;
    }
    else if (IsCaseEqual(Word, FOR_KEYWORD)) {
        return Tk_For;
    }
    else if (IsCaseEqual(Word, WHILE_KEYWORD)) {
        return Tk_While;
    }
    else if (IsCaseEqual(Word, INTEGER_KEYWORD)) {
        return Tk_Integer;
    }
    else if (IsCaseEqual(Word, FLOATING_KEYWORD)) {
        return Tk_Floating;
    }
    else if (IsCaseEqual(Word, USIZE_KEYWORD)) {
        return Tk_Usize;
    }
    else if (IsCaseEqual(Word, CHAR_KEYWORD)) {
        return Tk_Char;
    }
    else if (IsCaseEqual(Word, FUNCTION_KEYWORD)) {
        return Tk_Function;
    }
    else if (IsCaseEqual(Word, LOOP_KEYWORD)) {
        return Tk_Loop;
    }
    else if (IsCaseEqual(Word, BREAK_KEYWORD)) {
        return Tk_Break;
    }
    else if (IsCaseEqual(Word, CONTINUE_KEYWORD)) {
        return Tk_Continue;
    }
    else if (IsCaseEqual(Word, DEFINE_KEYWORD)) {
        return Tk_Define;
    }
    else if (IsCaseEqual(Word, IF_KEYWORD)) {
        return Tk_If;
    }
    else if (IsCaseEqual(Word, ELSE_IF_KEYWORD)) {
        return Tk_Elif;
    }
    else if (IsCaseEqual(Word, ELSE_KEYWORD)) {
        return Tk_Else;
    }
    else if (IsCaseEqual(Word, VOID_KEYWORD)) {
        return TK_Void;
    }
    else if (IsCaseEqual(Word, NULL_KEYWORD)) {
        return Tk_Null;
    }
    else if (IsCaseEqual(Word, END_OF_CODE_KEYWORD)) {
        return Tk_EndOfCode;
    }
    else 
    {
        return Tk_Indentifier;
    }
}

Alice_Token* ReadIndentifier(LexerState* AliceState) 
{
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while (IsIndentifierChar(AliceState -> Code -> Chars[Start + Length]) && !IsInvalid(ActualChar(AliceState))) 
    {
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    Alice_TokenType Type = Indentify(Word);
    return CreateToken(Type, Start, Length);
}

Alice_Token* ReadString(LexerState* AliceState)
{
    Next(AliceState, 1);
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while ((AliceState -> Code -> Chars[Start + Length]) != STRING_MARKER && !IsInvalid(ActualChar(AliceState))) 
    {
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));

    // Temporary
    printf("%s ", Word);
    Next(AliceState, 1);
    return CreateToken(Tk_String, Start, Length);
}

Alice_Token* ReadNumerical(LexerState* AliceState) 
{
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while (IsValidNumerical(AliceState -> Code -> Chars[Start + Length]) && !IsInvalid(ActualChar(AliceState))) 
    {
        Length++;
    }
    return CreateToken(Tk_Decimal, Start, Length);
}

Alice_Token* ReadArchiveMarker(LexerState* AliceState)
{
    Next(AliceState, 1);
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while ((AliceState -> Code -> Chars[Start + Length]) != ARCHIVE_MARKER && !IsInvalid(ActualChar(AliceState))) 
    {
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    Next(AliceState, 1);
    return CreateToken(Tk_ArchiveMarker, Start, Length);
    AliceState -> Line = 1;
}

void Tokenize(Alice_BigGirl* Girl, Vector* Tokens) 
{
    LexerState LocalState;
    LocalState.Char = '_';
    LocalState.Pointer = 0;
    LocalState.Line = 1;
    LocalState.Indentation = 0;

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