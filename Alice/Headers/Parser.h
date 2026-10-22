#ifndef _ParserH_
#define _ParserH_

#include "Headers/InterpreterTypes.h"
#include "Headers/Lexer.h"

typedef enum EnumInstructionType {
    AdressDeclr,
    OperationDeclr,
    AliasDeclr,
    PositionDeclr
} NodeType;

typedef struct StructParserNode {
    NodeType Type;
    Usize Line;
    union 
    {
    } Data;
} ParserNode;

typedef struct StructParserState 
{
    Vector* Tokens;
    Usize Pointer;
    Alice_Token* Actual;
} ParserState;

void Parsenize(Alice_BigGirl* BigGirl, Vector* Tokens);
#endif