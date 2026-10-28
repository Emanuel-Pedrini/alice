#ifndef _LexerH_
#define _LexerH_
#include "Headers/ArrayUtilities.h"
#include "Headers/CodeObject.h"

#define STRING_MARKER '\"' 
#define REGISTER_MARKER '.'
#define ARCHIVE_MARKER '`'

#define CONSTANT_KEYWORD "const"
#define VARIABLE_KEYWORD "var"
#define FOR_KEYWORD "for"
#define WHILE_KEYWORD "while"
#define FUNCTION_KEYWORD "fn"
#define LOOP_KEYWORD "loop"

#define BREAK_KEYWORD "break"
#define CONTINUE_KEYWORD "continue"
#define DEFINE_KEYWORD "define"
#define IF_KEYWORD "if"
#define RETURN_KEYWORD "return"
#define ELSE_IF_KEYWORD "elif"
#define ELSE_KEYWORD "else"
#define END_KEYWORD "end"

#define STATIC_KEYWORD "static"
#define LOCAL_KEYWORD "local"
#define GLOBAL_KEYWORD "global"
#define EXCEPT_KEYWORD "except"
#define TRY_KEYWORD "try"
#define DO_KEYWORD "do"
#define SELF_KEYWORD "self"
#define THEN_KEYWORD "then"

#define HAS_KEYWORD "has"
#define IS_KEYWORD "is"
#define GO_KEYWORD "go"
#define IN_KEYWORD "in"
#define AWAIT_KEYWORD "await"
#define AS_KEYWORD "as"

#define NOT_KEYWORD "not"
#define AND_KEYWORD "and"
#define OR_KEYWORD "or"

#define RANGE_KEYWORD "range"

#define ALICE_KEYWORD "alice"


#define END_OF_CODE_KEYWORD "__END_OF_CODE__"

typedef struct {
    
    String* Code;

    Usize Pointer;
    char Char;
    Usize Line;

} LexerState;

Vector* Tokenize(Alice_BigGirl* Girl);
Bool IsEqual(const char* Word, const char* Src);
void Skip(LexerState* AliceState);

typedef enum {
    Tk_Illegal = 0, 
        
    Tk_ArchiveMarker = 1,

    Tk_Adress = 2,     
    Tk_Colon = 3,     
    Tk_Point = 4,     
    Tk_Comma = 5,     
    Tk_Percentage = 6,    
    Tk_Asterisk = 7,    
    Tk_Commercial = 8,    
    Tk_Money = 9,
    Tk_Hashtag = 10,
    Tk_Bang = 11,
    Tk_Interrogation = 12,
    Tk_Backslash = 13,
    Tk_Pipe = 14,

    Tk_String = 15,

    Tk_LeftArrow = 16,
    Tk_RightArrow = 17,
    Tk_UpArrow = 18,

    Tk_Minus = 19,
    Tk_Plus = 20,
    Tk_Equal = 21,
    Tk_Div = 23,

    Tk_LeftParenthesis = 24,
    Tk_RightParenthesis = 25,
    Tk_LeftBrace = 26,
    Tk_RightBrace = 27,
    Tk_LeftKeys = 28,
    Tk_RightKeys = 29,

    Tk_Indentifier = 30,

    Tk_Indent = 31,
    Tk_Dedent = 32,

    Tk_LeftSetter = 33,
    Tk_RightSetter = 34,

    Tk_EqualEqual = 35,
    Tk_LessEqual = 36,
    Tk_GreaterEqual = 37,
    Tk_NotEqual = 38,
    Tk_DoubleColon = 39,

    Tk_For = 40,
    Tk_While = 41,
    Tk_Loop = 42,
    Tk_Break = 43,
    Tk_Continue = 44,

    Tk_Return = 45,

    Tk_Constant = 46,
    Tk_Define = 47,
    Tk_Variable = 48,

    Tk_If = 49,
    Tk_Elif = 50,
    Tk_Else = 51,

    Tk_Number = 52,
    Tk_Char = 55,
    Tk_Null = 57,
    
    Tk_Binary = 58,
    Tk_Decimal = 59,
    Tk_Hexadecimal = 60,

    Tk_NewLine = 63,
    Tk_Function = 64,
    Tk_End = 65,
    Tk_Do = 66,
    Tk_Static = 67,
    Tk_Local = 68,
    Tk_Global = 69,
    Tk_Except = 70,
    Tk_Try = 71,
    Tk_Self = 72,
    Tk_Then = 73,
    
    Tk_In = 75,
    Tk_Go = 76,
    Tk_Is = 80,
    Tk_Await = 82,
    Tk_As = 83,
    
    Tk_Alice = 89,
    Tk_DoublePoint = 92,
    Tk_TriplePoint = 93,
    Tk_Has = 94,

    Tk_And = 95,
    Tk_Or = 96,
    Tk_Not = 97,
    Tk_Range = 104,

    Tk_EndOfCode 

} Alice_TokenType;

typedef struct {
    Alice_TokenType Type;
    char* Literal;
    Usize Start;
    Usize Length;
    Usize Line;
} Alice_Token;

#endif