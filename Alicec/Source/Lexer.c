#include "Headers/CodeObject.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/Lexer.h"
#include "External/Libraries.h"

#define IfKeyword(KEYWORD, TOKEN) if (IsEqual(Word, KEYWORD)) {return TOKEN;}
#define Keyword(KEYWORD, TOKEN) else if (IsEqual(Word, KEYWORD)) {return TOKEN;}

IndentationHandler* NewIndentationHandler()
{
    IndentationHandler* IndHandler = __AlAllocate(sizeof(IndentationHandler));
    __AliceStack* V = __NewAlStack(sizeof(Usize));
    IndHandler -> IndentationHistory = V;
    IndHandler -> ActualIndentation = 0;
    IndHandler -> LastIndentation = 0;
    return IndHandler; 
}

Dsize IndentationChanged(IndentationHandler* IndHandler)
{
    if (IndHandler -> LastIndentation == IndHandler -> ActualIndentation) return 0;
    if (IndHandler -> LastIndentation < IndHandler -> ActualIndentation) return 1;
    if (IndHandler -> LastIndentation > IndHandler -> ActualIndentation) return -1;
}


char CurrentCharAliceState(LexerState* AliceState) {
    return AliceState -> Code -> Chars[AliceState -> Pointer];
}

char Peek(LexerState* AliceState, int Offset) {
    Usize Expected = AliceState -> Pointer + Offset;
    Usize Max = AliceState -> Code -> SSize;
    Usize Index = Expected < 0 ? 0 : Expected;
    Index = Index > Max ? Max : Index;
    return AliceState -> Code -> Chars[Index];
}

AliceToken* NewAliceToken(
    AliceTokenType Type,
    Usize Start,
    Usize Length,
    char* Literal
) 
{
    AliceToken* Buffer = __AlAllocate(sizeof(AliceToken));
    Buffer -> Type = Type;
    Buffer -> Start = Start;
    Buffer -> Length = Length;
    Buffer -> Literal = Literal;
    return Buffer;
}

__AliceString* ConversionToString__AliceToken(AliceToken* InQ)
{
    __AliceString* S =  __AlStringFrom("");
    if (InQ -> Type == ALICETK_EXPLICITCAST || InQ -> Type == ALICETK_COWARDCAST) __AppendAlString(S, "Cast<");
    __AppendAlString(S, __AlStringFrom(InQ -> Literal) -> Chars);
    if (InQ -> Type == ALICETK_EXPLICITCAST || InQ -> Type == ALICETK_COWARDCAST) __AppendAlString(S, ">");
    return S;
}

void Next(LexerState* AliceState, int Offset) {
    Usize Expected = AliceState -> Pointer + Offset;
    Usize Max = AliceState -> Code -> SSize;
    Usize Index = Expected < 0 ? 0 : Expected;
    Index = Index > Max ? Max : Index;
	AliceState -> Pointer = Index;
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
    Usize* IndentationCount = __AlAllocate(sizeof(Usize));
    *IndentationCount = CountIndentationAliceState(AliceState);
    AliceState -> IndHandler -> LastIndentation = AliceState -> IndHandler -> ActualIndentation;
    AliceState -> IndHandler -> ActualIndentation = *IndentationCount;

    Dsize IndentationChange = IndentationChanged(AliceState -> IndHandler);
    if (IndentationChange)
    {
        AliceToken* T;
        switch (IndentationChange)
        {
        case 1:
            T = NewAliceToken(ALICETK_INDENT, 0, 0, "<__INDENT__>");
            __PushAlStack(AliceState -> IndHandler -> IndentationHistory, IndentationCount);
            __PushAlVector(AliceState -> Tokens, T);
            break;
        case -1:
            Dsize DedentCounter = AliceState -> IndHandler -> IndentationHistory -> Elements -> VSize;
            while ((*(Usize*)__LookAlStack(AliceState -> IndHandler -> IndentationHistory)) > (*IndentationCount) && DedentCounter > 0)
            {
                DedentCounter--;
                __PopAlStack(AliceState -> IndHandler -> IndentationHistory);
                T = NewAliceToken(ALICETK_DEDENT, 0, 0, "<__DEDENT__>");
                __PushAlVector(AliceState -> Tokens, T);
            }
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
            while (!(CurrentCharAliceState(AliceState) == '*' && Peek(AliceState, 1) == '/') && !IsInvalid(CurrentCharAliceState(AliceState))) 
            {                
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
            if (AliceState -> Depth == 0)
            {
                AliceToken* T = NewAliceToken(ALICETK_NEWLINE, 0, 0, "");
                __PushAlVector(AliceState -> Tokens, T);
            }
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
    AliceTokenType Type = ALICETK_ILLEGAL;
    Usize Start = AliceState -> Pointer;
    Usize Length = 1;

    switch (CurrentCharAliceState(AliceState)) {
        case '@':
            Type = ALICETK_ATSIGN;
            break;

        case '(':
            Type = ALICETK_LEFTPARENTHESIS;
            AliceState -> Depth++;
            break;
        case ')':
            Type = ALICETK_RIGHTPARENTHESIS;
            AliceState -> Depth--;
            break;
        case '{':
            Type = ALICETK_LEFTKEYS;
            AliceState -> Depth++;
            break;
        case '~':
            Type = ALICETK_TILDE;
            break;
        case '}':
            Type = ALICETK_RIGHTKEYS;
            AliceState -> Depth--;
            break;
        case '[':
            Type = ALICETK_LEFTBRACE;
            AliceState -> Depth++;
            break;
        case ']':
            Type = ALICETK_RIGHTBRACE;
            AliceState -> Depth--;
            break;

        case '%':
            Type = ALICETK_PERCENT;
            break;
        case '$':
            Type = ALICETK_DOLLARSIGN;
            break;

        case '?':
            Type = ALICETK_INTERROGATION;
            break;

        case '*':
            Type = ALICETK_ASTERISK;
            break;
            
        case '<':
            switch (Peek(AliceState, 1))
            {
                case '-':
                    Type = ALICETK_LEFTSETTER;
                    Length = 2;
                    break;
                case '=':
                    Type = ALICETK_LESSEQUAL;
                    Length = 2;
                    break;
                default:
                    Type = ALICETK_LEFTARROW;
                    break;
            }
            break;
        case '>':
            switch (Peek(AliceState, 1))
            {
                case '=':
                    Type = ALICETK_GREATEREQUAL;
                    Length = 2;
                    break;
                default:
                    Type = ALICETK_RIGHTARROW;
                    break;
            }
            break;

        case '!':
            switch (Peek(AliceState, 1))
            {
                case '=':
                    Type = ALICETK_NOTEQUAL;
                    Length = 2;
                    break;
                default:
                    Type = ALICETK_BANG;
                    break;
            }
            break;

        case '^':
            Type = ALICETK_UPARROW;
            break;

        case '+':
            Type = ALICETK_PLUS;

            break;
        case '-':
            switch (Peek(AliceState, 1))
            {
                case '>':
                    Type = ALICETK_RIGHTSETTER;
                    Length = 2;
                    break;

                default:
                    Type = ALICETK_MINUS;
                    break;
            }
            break;
        case ':':
            switch (Peek(AliceState, 1))
            {
                case ':':
                    Type = ALICETK_DOUBLECOLON;
                    Length = 2;
                    break;

                default:
                    Type = ALICETK_COLON;
                    break;
            }
            break;

        case '=':
            switch (Peek(AliceState, 1))
            {
                case '=':
                    Type = ALICETK_EQUALEQUAL;
                    Length = 2;
                    break;

                default:
                    Type = ALICETK_EQUAL;
                    break;
            }
            break;

        case '/':
            Type = ALICETK_DIV;
            break;

        case '\\':
            Type = ALICETK_BACKSLASH;
            break;
        case '&':
            Type = ALICETK_AMPERSAND;
            break;
        case '|':
            switch (Peek(AliceState, 1))
            {
                default:
                    Type = ALICETK_PIPE;
                    break;
            }
            break;
        case ',':
            Type = ALICETK_COMMA;
            break;

        case '.':
            switch (Peek(AliceState, 1))
            {
				case '.':
					switch (Peek(AliceState, 2))
					{
						case '.':
							Type = ALICETK_TRIPLEPOINT;
							Length = 3;
							break;

						default:
							Type = ALICETK_DOUBLEPOINT;
							Length = 2;
							break;
					}
					break;
                default:
                    Type = ALICETK_POINT;
                    break;
            }
            break;

        default:
            Type = ALICETK_ILLEGAL;
            break;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    return NewAliceToken(Type, Start, Length, Word);
}

AliceTokenType HandleFreewordAliceState(LexerState* AliceState, CString Word, uint32_t Length) 
{
	IfKeyword(RETURN_KEYWORD, ALICETK_RETURN)
	Keyword(DO_KEYWORD, ALICETK_DO)
	Keyword(DEFINE_KEYWORD, ALICETK_DEFINE)
	Keyword(CONSTANT_KEYWORD, ALICETK_CONSTANT)
	Keyword(VARIABLE_KEYWORD, ALICETK_VARIABLE)
	Keyword(FOR_KEYWORD, ALICETK_FOR)
	Keyword(WHILE_KEYWORD, ALICETK_WHILE)
	Keyword(FUNCTION_KEYWORD, ALICETK_FUNCTION)
	Keyword(LOOP_KEYWORD, ALICETK_LOOP)
	Keyword(BREAK_KEYWORD, ALICETK_BREAK)
	Keyword(CONTINUE_KEYWORD, ALICETK_CONTINUE)
	Keyword(IF_KEYWORD, ALICETK_IF)
	Keyword(ELSE_IF_KEYWORD, ALICETK_ELIF)
	Keyword(ELSE_KEYWORD, ALICETK_ELSE)
	Keyword(END_KEYWORD, ALICETK_END)
	else if (IsCaseEqual(Word, "static") && Peek(AliceState, Length) != '<') 
        {
            return ALICETK_STATIC;
        }
	Keyword(LOCAL_KEYWORD, ALICETK_LOCAL)
	Keyword(GLOBAL_KEYWORD, ALICETK_GLOBAL)
	Keyword(EXCEPT_KEYWORD, ALICETK_EXCEPT)
	Keyword(TRY_KEYWORD, ALICETK_TRY)
	Keyword(SELF_KEYWORD, ALICETK_SELF)
	Keyword(THEN_KEYWORD, ALICETK_THEN)
	Keyword(IS_KEYWORD, ALICETK_IS)
	Keyword(IN_KEYWORD, ALICETK_IN)
	Keyword(GO_KEYWORD, ALICETK_GO)
	Keyword(AWAIT_KEYWORD, ALICETK_AWAIT)
	Keyword(END_OF_CODE_KEYWORD, ALICETK_ENDOFCODE)
	Keyword(AS_KEYWORD, ALICETK_AS)
	Keyword(ALICE_KEYWORD, ALICETK_ALICE)
	Keyword(NOT_KEYWORD, ALICETK_NOT)
	Keyword(AND_KEYWORD, ALICETK_AND)
	Keyword(OR_KEYWORD, ALICETK_OR)
    Keyword(STRUCT_KEYWORD, ALICETK_STRUCT)
    else {return ALICETK_IDENTIFIER;}
}

AliceToken* ReadIdentifier(LexerState* AliceState) 
{
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while (IsIdentifierChar(AliceState -> Code -> Chars[Start + Length]) && !IsInvalid(CurrentCharAliceState(AliceState))) 
    {
        Length++;
    }
    char* Word = Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    AliceTokenType Type = HandleFreewordAliceState(AliceState, Word, Length);
    
    Dsize Offset = -1;
    switch (Peek(AliceState, Offset))
    {
        case '!': 
        {
            if (Offset < 0) {__PopAlVector(AliceState -> Tokens); return NewAliceToken(ALICETK_EXPLICITCAST, Start, Length, Word);}
            return NewAliceToken(ALICETK_EXPLICITCAST, Start, Length + 1, Word);
        }
        case '?': 
        {
            if (Offset < 0) {__PopAlVector(AliceState -> Tokens); return NewAliceToken(ALICETK_COWARDCAST, Start, Length, Word);}
            return NewAliceToken(ALICETK_COWARDCAST, Start, Length + 1, Word);
        }
        default: return NewAliceToken(Type, Start, Length, Word);
    }
}

AliceToken* ReadString(LexerState* AliceState)
{
    Next(AliceState, 1);
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while ((
        AliceState -> Code -> Chars[Start + Length]) != STRING_MARKER[0])
    {
        if (AliceState -> Code -> Chars[Start + Length] == '\0')
        {
            break;
        }
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    __AliceString* S = __AlStringFrom("");
    for (int i = 0;Word[i] != '\0';i++)
    {
        if (Word[i] == '\\')
        {
            char Next = Word[i + 1];
            switch (Next)
            {
                case 'n':__AddCharAlString(S, '\n');i++;continue;
                case 't':__AddCharAlString(S, '\t');i++;continue;
                case '\"':__AddCharAlString(S, '\"');i++;continue;
                case 'a':__AddCharAlString(S, '\a');i++;continue;
                case 'b':__AddCharAlString(S, '\b');i++;continue;
                case '\'':__AddCharAlString(S, '\'');i++;continue;
                default:
                    continue;
            }
        }
        __AddCharAlString(S, Word[i]);
    }

    Next(AliceState, 1);
    return NewAliceToken(ALICETK_STRING, Start, Length, S -> Chars);
}

AliceToken* ReadNumerical(LexerState* AliceState) 
{
    int Length = 0;
    AliceTokenType Type = ALICETK_NUMBER;
    Usize Start = AliceState -> Pointer;
    Usize FloatingPoints = 0;
    while (IsValidNumerical(AliceState -> Code -> Chars[Start + Length]) || IsValidNumerical(AliceState -> Code -> Chars[Start + Length] == '.') && !IsInvalid(CurrentCharAliceState(AliceState))) 
    {
        if (AliceState -> Code -> Chars[Start + Length] == '.')
        {
            FloatingPoints++;
            if (FloatingPoints <= 1) Type = ALICETK_DECIMAL;
            if (FloatingPoints > 1)
            {
                // Read a range expression.
            }
        }
        Length++;
    }
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    return NewAliceToken(Type, Start, Length, Word);
}

AliceToken* ReadArchiveMarker(LexerState* AliceState)
{
    Next(AliceState, 1);
    int Length = 0;
    Usize Start = AliceState -> Pointer;
    while ((
        AliceState -> Code -> Chars[Start + Length]) != ARCHIVE_MARKER[0] 
        && AliceState -> Code -> Chars[Start + Length] != '\0') 
    {
        Length++;
    }
    
    AliceState -> Line = 1;
    
    char* Word =  Substring(AliceState -> Code -> Chars, Start, (Start + Length));
    Next(AliceState, 1);

    return NewAliceToken(ALICETK_ARCHIVE, Start, Length, Word);
}

__AliceVector* AliceLexerTokenize(AliceManagementObjectRef ManagementObject) 
{
	__AliceVector* Tokens = __NewAlVector(sizeof(AliceToken));
    LexerState LocalState;
    LocalState.Char = '_';
    LocalState.Pointer = 0;
    LocalState.Line = 1;
    LocalState.Depth = 0;
    LocalState.Code = ManagementObject -> SourceCode;
    LocalState.Tokens = Tokens;
    LocalState.IndHandler = NewIndentationHandler();

    for (int x = 0; CurrentCharAliceState(&LocalState) != '\0';x++) 
    {
        HandleNonStandardAliceState(&LocalState);
		
        if (IsInvalid(CurrentCharAliceState(&LocalState))) break;

        Usize Offset = 1;
        AliceToken* LocalToken = NULL;

        if (CurrentCharAliceState(&LocalState) == ARCHIVE_MARKER[0]) 
        {
            LocalToken = ReadArchiveMarker(&LocalState);
        }
        else if (CurrentCharAliceState(&LocalState) == STRING_MARKER[0]) 
        {
            LocalToken = ReadString(&LocalState);
        }
        else if (!IsIdentifierChar(CurrentCharAliceState(&LocalState))) 
        {
            LocalToken = HandleSymbolAliceState(&LocalState);
        }
        else if (IsNumerical(CurrentCharAliceState(&LocalState))) 
        {
            LocalToken = ReadNumerical(&LocalState);
        }
        else if (IsIdentifierChar(CurrentCharAliceState(&LocalState)))
        {
            LocalToken = ReadIdentifier(&LocalState);
        }

        if (LocalToken != NULL) {
            Offset = LocalToken -> Length;
            LocalToken -> Line = LocalState.Line;
            __PushAlVector(LocalState.Tokens, LocalToken);
        }
        Next(&LocalState, Offset);
    }
    // AliceToken* FinalNewLine = NewAliceToken(ALICETK_NEWLINE, LocalState.Pointer, 0, "");
    // __PushAlVector(Tokens, FinalNewLine);
    HandleLineAliceState(&LocalState);
    AliceToken* EndingToken = NewAliceToken(ALICETK_ENDOFCODE, LocalState.Pointer, 0, "__END_OF_CODE__");
    __PushAlVector(Tokens, EndingToken);
	return Tokens;
}