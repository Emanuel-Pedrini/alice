#include "Headers/CodeObject.h"
#include "Headers/ArrayUtilities.h"
#include "Headers/StringUtilities.h"
#include "Headers/Lexer.h"
#include "External/Libraries.h"

#define IfKeyword(KEYWORD, TOKEN) if (IsCaseEqual(Word, KEYWORD)) {return TOKEN;}
#define Keyword(KEYWORD, TOKEN) else if (IsCaseEqual(Word, KEYWORD)) {return TOKEN;}

IndentationHandler* NewIndentationHandler()
{
    IndentationHandler* IndHandler = malloc(sizeof(IndentationHandler));
    VECTOR(V, Usize)
    IndHandler -> IndentationHistory = V;
    IndHandler -> ActualIndentation = 0;
    IndHandler -> LastIndentation = 0;
    return IndHandler; 
}

Usize IndentationChanged(IndentationHandler* IndHandler)
{
    if (IndHandler -> LastIndentation == IndHandler -> ActualIndentation) return 0;
    if (IndHandler -> LastIndentation < IndHandler -> ActualIndentation) return 1;
    if (IndHandler -> LastIndentation > IndHandler -> ActualIndentation) return -1;
}


char CurrentCharAliceState(LexerState* AliceState) {
    return AliceState -> Code -> Chars[AliceState -> Pointer];
}

char Peek(LexerState* AliceState, int Offset) {
    return AliceState -> Code -> Chars[AliceState -> Pointer + Offset];
}

AliceToken* NewAliceToken(
    AliceTokenType Type,
    Usize Start,
    Usize Length,
    char* Literal
) 
{
    AliceToken* Buffer = malloc(sizeof(AliceToken));
    Buffer -> Type = Type;
    Buffer -> Start = Start;
    Buffer -> Length = Length;
    Buffer -> Literal = Literal;
    Buffer -> ConversionToString = ConversionToString__AliceToken;
    return Buffer;
}

String* ConversionToString__AliceToken(AliceToken* InQ)
{
    String* S =  FromStr("");
    AppendStr(S, FromStr(InQ -> Literal) -> Chars);
    AppendStr(S, "");
    return S;
}

void Next(LexerState* AliceState, int Offset) {
	Usize NextPointer = AliceState -> Pointer + Offset;
	if (IsInvalid(AliceState -> Code -> Chars[NextPointer]))
	{
    	AliceState -> Pointer = NextPointer;
	}
	AliceState -> Pointer = NextPointer;
}

Bool IsOneLineCommentaryAliceState(LexerState* AliceState)
{
    if (CurrentCharAliceState(AliceState) == '/' && Peek(AliceState, 1) == '/')
    {
        return True;
    }
    return False;
}

Usize CountIndentationAliceState(LexerState* AliceState)
{
    Usize SpaceCount = 0;
    while (CurrentCharAliceState(AliceState) == ' ')
    {
        SpaceCount++;
        Next(AliceState, 1);
    }
    if (IsOneLineCommentaryAliceState(AliceState))
    {
        return AliceState -> IndHandler -> ActualIndentation;
    }
    return SpaceCount;
}

void HandleLineAliceState(LexerState* AliceState)
{
    char LastCharacter = Peek(AliceState, -1);
    
    AliceState -> Line++;
    Next(AliceState, 1);
    Usize* IndentationCount = malloc(sizeof(Usize));
    *IndentationCount = CountIndentationAliceState(AliceState);
    AliceState -> IndHandler -> LastIndentation = AliceState -> IndHandler -> ActualIndentation;
    AliceState -> IndHandler -> ActualIndentation = *IndentationCount;
    Add(AliceState -> IndHandler -> IndentationHistory, IndentationCount);

    Usize IndentationChange = IndentationChanged(AliceState -> IndHandler);
    if (IndentationChange)
    {
        AliceToken* T;
        switch (IndentationChange)
        {
        case 1:
            T = NewAliceToken(Tk_Indent, 0, 0, "Indent");
            Add(AliceState -> Tokens, T);
            break;
        case -1:
            T = NewAliceToken(Tk_Dedent, 0, 0, "Dedent");
            Add(AliceState -> Tokens, T);
            break;
        default:
            break;
        }
    }
}

void HandleNonStandardAliceState(LexerState* AliceState) {
    while (1) 
    {
        if (IsOneLineCommentaryAliceState(AliceState))
        {
            while (CurrentCharAliceState(AliceState) != '\n' && !IsInvalid(CurrentCharAliceState(AliceState))) 
            {
                Next(AliceState, 1);
            }
            HandleLineAliceState(AliceState);
            continue;
        }

        if ((CurrentCharAliceState(AliceState) == '/' && Peek(AliceState, 1) == '*') && !IsInvalid(CurrentCharAliceState(AliceState))) 
        {
            while (!(CurrentCharAliceState(AliceState) == '*' && Peek(AliceState, 1) == '/')) {                
                Next(AliceState, 1);
            }
            Next(AliceState, 2);
            continue;
        }

        if ((CurrentCharAliceState(AliceState) == ' ') && !IsInvalid(CurrentCharAliceState(AliceState))) 
        { 
            Next(AliceState, 1);
            continue;
        }

        if (CurrentCharAliceState(AliceState) == '\n' && !IsInvalid(CurrentCharAliceState(AliceState)))
        {
            AliceToken* T = NewAliceToken(Tk_NewLine, 0, 0, "");
            Add(AliceState -> Tokens, T);
            HandleLineAliceState(AliceState);
            continue;
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

AliceToken* HandleSymbolAliceState(LexerState* AliceState) 
{
    AliceTokenType Type = Tk_Illegal;
    Usize Start = AliceState -> Pointer;
    Usize Length = 1;

    switch (CurrentCharAliceState(AliceState)) {
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
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    return NewAliceToken(Type, Start, Length, Word);
}

AliceTokenType HandleFreewordAliceState(const char* Word) 
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

AliceToken* ReadIndentifier(LexerState* AliceState) 
{
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while (IsIndentifierChar(AliceState -> Code -> Chars[Start + Length]) && !IsInvalid(CurrentCharAliceState(AliceState))) 
    {
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    AliceTokenType Type = HandleFreewordAliceState(Word);
    return NewAliceToken(Type, Start, Length, Word);
}

AliceToken* ReadString(LexerState* AliceState)
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
    return NewAliceToken(Tk_String, Start, Length, Word);
}

AliceToken* ReadNumerical(LexerState* AliceState) 
{
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while (IsValidNumerical(AliceState -> Code -> Chars[Start + Length]) && !IsInvalid(CurrentCharAliceState(AliceState))) 
    {
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    return NewAliceToken(Tk_Decimal, Start, Length, Word);
}

AliceToken* ReadArchiveMarker(LexerState* AliceState)
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
    
    AliceState -> Line = 1;
    
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    Next(AliceState, 1);

    return NewAliceToken(Tk_ArchiveMarker, Start, Length, Word);
}

Vector* AliceLexerTokenize(String* SourceCode) 
{
	VECTOR(Tokens, AliceToken)
    LexerState LocalState;
    LocalState.Char = '_';
    LocalState.Pointer = 0;
    LocalState.Line = 1;
    LocalState.Code = SourceCode;
    LocalState.Tokens = Tokens;
    LocalState.IndHandler = NewIndentationHandler();

    for (int x = 0; CurrentCharAliceState(&LocalState) != '\0';x++) 
    {
        HandleNonStandardAliceState(&LocalState);
		
        if (IsInvalid(CurrentCharAliceState(&LocalState))) break;

        Usize Offset = 1;
        AliceToken* LocalToken = NULL;

        if (CurrentCharAliceState(&LocalState) == ARCHIVE_MARKER) 
        {
            LocalToken = ReadArchiveMarker(&LocalState);
        }
        else if (CurrentCharAliceState(&LocalState) == STRING_MARKER) 
        {
            LocalToken = ReadString(&LocalState);
        }
        else if (!IsIndentifierChar(CurrentCharAliceState(&LocalState))) 
        {
            LocalToken = HandleSymbolAliceState(&LocalState);
        }
        else if (IsNumerical(CurrentCharAliceState(&LocalState))) 
        {
            LocalToken = ReadNumerical(&LocalState);
        }
        else if (IsIndentifierChar(CurrentCharAliceState(&LocalState)))
        {
            LocalToken = ReadIndentifier(&LocalState);
        }

        if (LocalToken != NULL) {
            Offset = LocalToken -> Length;
            LocalToken -> Line = LocalState.Line;
            Add(LocalState.Tokens, LocalToken);
        }
        Next(&LocalState, Offset);
    }
    AliceToken* END_TOKEN = NewAliceToken(Tk_EndOfCode, LocalState.Pointer, 0, "__END_OF_CODE__");
    Add(Tokens, END_TOKEN);
	return Tokens;
}