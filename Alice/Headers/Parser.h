#ifndef _ParserH_
#define _ParserH_

#include "Headers/InterpreterTypes.h"
#include "Headers/Lexer.h"

struct StructParserNode;
struct StructAliceType;
struct Block;

typedef enum EnumInstructionType {
    AdressDeclr,
    OperationDeclr,
    AliasDeclr,
    PositionDeclr
} NodeType;

typedef struct StructAliceType 
{

} AliceType;

typedef struct StructBlock
{
    Vector* Nodes;
} Block;

typedef struct StructParserNode {
    NodeType Type;
    Usize Line;
    union 
    {
        struct StructFunctionArgument 
        {
            String* Name;
            AliceType* Type;
        } FunctionArgument;
        struct StructFunction
        {
            String* Name;
            AliceType* ReturnType;
        } Function;
    } Data;
} ParserNode;

typedef struct StructParserState 
{
    Vector* Tokens;
    Usize Pointer;
    Alice_Token* Actual;
} ParserState;

Vector* Parsenize(Alice_BigGirl* BigGirl, Vector* Tokens);
#endif