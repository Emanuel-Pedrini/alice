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
    ALICETK_ILLEGAL, 
        
    ALICETK_ARCHIVE,

    ALICETK_ATSIGN,     
    ALICETK_COLON,     
    ALICETK_POINT,     
    ALICETK_COMMA,     
    ALICETK_PERCENT,    
    ALICETK_ASTERISK,    
    ALICETK_AMPERSAND,    
    ALICETK_DOLLARSIGN,
    ALICETK_HASHTAG,
    ALICETK_BANG,
    ALICETK_INTERROGATION,
    ALICETK_BACKSLASH,
    ALICETK_PIPE,

    ALICETK_LEFTARROW,
    ALICETK_RIGHTARROW,
    ALICETK_UPARROW,

    ALICETK_MINUS,
    ALICETK_PLUS,
    ALICETK_EQUAL,
    ALICETK_DIV,

    ALICETK_LEFTPARENTHESIS,
    ALICETK_RIGHTPARENTHESIS,
    ALICETK_LEFTBRACE,
    ALICETK_RIGHTBRACE,
    ALICETK_LEFTKEYS,
    ALICETK_RIGHTKEYS,

    ALICETK_IDENTIFIER,

    ALICETK_INDENT,
    ALICETK_DEDENT,

    ALICETK_LEFTSETTER,
    ALICETK_RIGHTSETTER,

    ALICETK_EQUALEQUAL,
    ALICETK_LESSEQUAL,
    ALICETK_GREATEREQUAL,
    ALICETK_NOTEQUAL,
    ALICETK_DOUBLECOLON,


    ALICETK_FOR,
    ALICETK_WHILE,
    ALICETK_LOOP,
    ALICETK_BREAK,
    ALICETK_CONTINUE,

    ALICETK_RETURN,

    ALICETK_CONSTANT,
    ALICETK_DEFINE,
    ALICETK_VARIABLE,

    ALICETK_IF,
    ALICETK_ELIF,
    ALICETK_ELSE,

    ALICETK_NUMBER,
    ALICETK_CHAR,
    ALICETK_NULL,
    
    ALICETK_BINARY,
    ALICETK_DECIMAL,
    ALICETK_HEXADECIMAL,

    ALICETK_NEWLINE,
    ALICETK_FUNCTION,
    ALICETK_END,
    ALICETK_DO,
    ALICETK_STATIC,
    ALICETK_LOCAL,
    ALICETK_GLOBAL,
    ALICETK_EXCEPT,
    ALICETK_TRY,
    ALICETK_SELF,
    ALICETK_THEN,
    
    ALICETK_IN,
    ALICETK_GO,
    ALICETK_IS,
    ALICETK_AWAIT,
    ALICETK_AS,
    
    ALICETK_ALICE,
    ALICETK_DOUBLEPOINT,
    ALICETK_TRIPLEPOINT,
    
    ALICETK_AND,
    ALICETK_OR,
    ALICETK_NOT,
    
    ALICETK_STRING,
    
    ALICETK_TILDE,
    ALICETK_STRUCT,
    ALICETK_ANYTHING,
    ALICETK_EXPLICITCAST,
    ALICETK_COWARDCAST,
    ALICETK_ENDOFCODE 

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