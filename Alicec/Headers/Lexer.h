#ifndef _LexerH_
#define _LexerH_

#include "Headers/CodeObject.h"
#include "Headers/CStringUtils.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/ForwardInclude.h"

#define STRING_MARKER "\""
#define REGISTER_MARKER "."
#define ARCHIVE_MARKER "`"
#define INDENTATION_MARKER ":"
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
#define STRUCT_KEYWORD "struct"
#define ALICE_KEYWORD "alice"
#define END_OF_CODE_KEYWORD "__END_OF_CODE__"

typedef struct Struct__AliceToken AliceToken;
typedef struct Struct__LexerState LexerState;
typedef struct Struct__IndentationHandler IndentationHandler;
typedef enum Enum__AliceTokenType AliceTokenType;

struct Struct__IndentationHandler
{
    __AliceStack* IndentationHistory;
    Usize LastIndentation;
    Usize ActualIndentation;
};

IndentationHandler* NewIndentationHandler();
Dsize IndentationChanged(IndentationHandler* IndHandler);

struct Struct__LexerState {
    __AliceString* Code;
    Usize Pointer;
    char Char;
    Usize Line;
    Usize Depth;
    IndentationHandler* IndHandler;
    __AliceVector* Tokens;
};

enum Enum__AliceTokenType {
    AliceTK_Illegal, 
        
    AliceTK_Archive,

    AliceTK_AtSign,     
    AliceTK_Colon,     
    AliceTK_Point,     
    AliceTK_Comma,     
    AliceTK_Percent,    
    AliceTK_Asterisk,    
    AliceTK_Ampersand,    
    AliceTK_DollarSign,
    AliceTK_Hashtag,
    AliceTK_Bang,
    AliceTK_Interrogation,
    AliceTK_Backslash,
    AliceTK_Pipe,

    Tk_LeftArrow,
    Tk_RightArrow,
    Tk_UpArrow,

    Tk_Minus,
    Tk_Plus,
    Tk_Equal,
    Tk_Div,

    Tk_LeftParenthesis,
    Tk_RightParenthesis,
    Tk_LeftBrace,
    Tk_RightBrace,
    Tk_LeftKeys,
    Tk_RightKeys,

    Tk_Identifier,

    Tk_Indent,
    Tk_Dedent,

    Tk_LeftSetter,
    Tk_RightSetter,

    Tk_EqualEqual,
    Tk_LessEqual,
    Tk_GreaterEqual,
    Tk_NotEqual,
    Tk_DoubleColon,


    Tk_For,
    Tk_While,
    Tk_Loop,
    Tk_Break,
    Tk_Continue,

    Tk_Return,

    Tk_Constant,
    Tk_Define,
    Tk_Variable,

    Tk_If,
    Tk_Elif,
    Tk_Else,

    Tk_Number,
    Tk_Char,
    Tk_Null,
    
    Tk_Binary,
    Tk_Decimal,
    Tk_Hexadecimal,

    Tk_NewLine,
    Tk_Function,
    Tk_End,
    Tk_Do,
    Tk_Static,
    Tk_Local,
    Tk_Global,
    Tk_Except,
    Tk_Try,
    Tk_Self,
    Tk_Then,
    
    Tk_In,
    Tk_Go,
    Tk_Is,
    Tk_Await,
    Tk_As,
    
    Tk_Alice,
    Tk_DoublePoint,
    Tk_TriplePoint,
    
    Tk_And,
    Tk_Or,
    Tk_Not,
    
    Tk_String,
    
    Tk_Tilde,
    Tk_Struct,
    Tk_Anything,
    Tk_ExplicitCast,
    Tk_CowardCast,
    Tk_EndOfCode 

};

struct Struct__AliceToken  {
    AliceTokenType Type;
    char* Literal;
    Usize Start;
    Usize Length;
    Usize Line;
};

AliceToken* NewAliceToken(AliceTokenType Type, Usize Start, Usize Length, char* Literal);
__AliceString* ConversionToString__AliceToken(AliceToken* InQ);


__AliceVector* AliceLexerTokenize(AliceManagementObjectRef ManagementObject);
Bool IsEqual(CString Word, CString Src);
void Skip(LexerState* AliceState);
AliceToken* ReadIdentifier(LexerState* AliceState);
AliceTokenType HandleFreewordAliceState(LexerState* AliceState, CString Word, uint32_t Length);
#endif