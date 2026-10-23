#include "Headers/CodeObject.h"
#include "Headers/ArrayUtilities.h"
#include "Headers/StringUtilities.h"
#include "Headers/Lexer.h"
#include "External/Libraries.h"

#define IfKeyword(KEYWORD, TOKEN) if (IsCaseEqual(Word, KEYWORD)) {return TOKEN;}
#define Keyword(KEYWORD, TOKEN) else if (IsCaseEqual(Word, KEYWORD)) {return TOKEN;}

char ActualChar(LexerState* AliceState) {
    return AliceState -> Code -> Chars[AliceState -> Pointer];
}

char Peek(LexerState* AliceState, int Offset) {
    return AliceState -> Code -> Chars[AliceState -> Pointer + Offset];
}

Alice_Token* CreateToken(
    Alice_TokenType Type,
    Usize Start,
    Usize Length,
    char* Literal
) 
{
    Alice_Token* Buffer = malloc(sizeof(Alice_Token));
    Buffer -> Type = Type;
    Buffer -> Start = Start;
    Buffer -> Length = Length;
    Buffer -> Literal = Literal;
    return Buffer;
}

void Next(LexerState* AliceState, int Offset) {
	Usize NextPointer = AliceState -> Pointer + Offset;
	if (IsInvalid(AliceState -> Code -> Chars[NextPointer]))
	{
    	AliceState -> Pointer = NextPointer;
	}
	AliceState -> Pointer = NextPointer;
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

        if ((ActualChar(AliceState) == ' ' || ActualChar(AliceState) == '\n') && !IsInvalid(ActualChar(AliceState))) 
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

        case '.':
            switch (Peek(AliceState, 1))
            {
				case '.':
					switch (Peek(AliceState, 2))
					{
						case '.':
							Type = Tk_TriplePoint;
							Length = 3;
							break;

						default:
							Type = Tk_DoublePoint;
							Length = 2;
							break;
					}
					break;
                default:
                    Type = Tk_Point;
                    break;
            }
            break;

        default:
            Type = Tk_Illegal;
            break;
    }
    return CreateToken(Type, Start, Length, "");
}

Alice_TokenType Indentify(const char* Word) 
{
	IfKeyword(RETURN_KEYWORD, Tk_Return)
	Keyword(DO_KEYWORD, Tk_Do)
	Keyword(DEFINE_KEYWORD, Tk_Define)
	Keyword(CONSTANT_KEYWORD, Tk_Constant)
	Keyword(VARIABLE_KEYWORD, Tk_Variable)
	Keyword(FOR_KEYWORD, Tk_For)
	Keyword(WHILE_KEYWORD, Tk_While)
	Keyword(FUNCTION_KEYWORD, Tk_Function)
	Keyword(LOOP_KEYWORD, Tk_Loop)
	Keyword(BREAK_KEYWORD, Tk_Break)
	Keyword(CONTINUE_KEYWORD, Tk_Continue)
	Keyword(IF_KEYWORD, Tk_If)
	Keyword(ELSE_IF_KEYWORD, Tk_Elif)
	Keyword(ELSE_KEYWORD, Tk_Else)
	Keyword(END_KEYWORD, Tk_End)
	Keyword(STATIC_KEYWORD, Tk_Static)
	Keyword(LOCAL_KEYWORD, Tk_Local)
	Keyword(GLOBAL_KEYWORD, Tk_Global)
	Keyword(EXCEPT_KEYWORD, Tk_Except)
	Keyword(TRY_KEYWORD, Tk_Try)
	Keyword(SELF_KEYWORD, Tk_Self)
	Keyword(THEN_KEYWORD, Tk_Then)
	Keyword(IS_KEYWORD, Tk_Is)
	Keyword(IN_KEYWORD, Tk_In)
	Keyword(GO_KEYWORD, Tk_Go)
	Keyword(AWAIT_KEYWORD, Tk_Await)
	Keyword(END_OF_CODE_KEYWORD, Tk_EndOfCode)
	Keyword(AS_KEYWORD, Tk_As)
	Keyword(ALICE_KEYWORD, Tk_Alice)
	Keyword(HAS_KEYWORD, Tk_Has)
	Keyword(NOT_KEYWORD, Tk_Not)
	Keyword(AND_KEYWORD, Tk_And)
	Keyword(OR_KEYWORD, Tk_Or)
	Keyword(RANGE_KEYWORD, Tk_Range)
    else {return Tk_Indentifier;}
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
    return CreateToken(Type, Start, Length, Word);
}

Alice_Token* ReadString(LexerState* AliceState)
{
    Next(AliceState, 1);
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while ((
        AliceState -> Code -> Chars[Start + Length]) != STRING_MARKER)
    {
        if (AliceState -> Code -> Chars[Start + Length] == '\0')
        {
            printf("Unclosed string!");
            break;
        }
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));

    Next(AliceState, 1);
    return CreateToken(Tk_String, Start, Length, Word);
}

Alice_Token* ReadNumerical(LexerState* AliceState) 
{
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while (IsValidNumerical(AliceState -> Code -> Chars[Start + Length]) && !IsInvalid(ActualChar(AliceState))) 
    {
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    return CreateToken(Tk_Decimal, Start, Length, Word);
}

Alice_Token* ReadArchiveMarker(LexerState* AliceState)
{
    Next(AliceState, 1);
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while ((
        AliceState -> Code -> Chars[Start + Length]) != ARCHIVE_MARKER 
        && AliceState -> Code -> Chars[Start + Length] != '\0') 
    {
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    Next(AliceState, 1);
    return CreateToken(Tk_ArchiveMarker, Start, Length, Word);
    AliceState -> Line = 1;
}

Vector* Tokenize(Alice_BigGirl* Girl) 
{
	Vec(Tokens, Alice_Token)
    LexerState LocalState;
    LocalState.Char = '_';
    LocalState.Pointer = 0;
    LocalState.Line = 1;

    LocalState.Code = Girl -> SourceCode;
    for (int x = 0; ActualChar(&LocalState) != '\0';x++) {
        Skip(&LocalState);
		if (IsInvalid(ActualChar(&LocalState))) break;

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
    Alice_Token END_TOKEN = {Tk_EndOfCode, "__END_OF_CODE__", LocalState.Pointer, 0};
    Add(Tokens, &END_TOKEN);
	return Tokens;
}