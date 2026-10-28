#ifndef _ParserH_
#define _ParserH_

#include "Headers/InterpreterTypes.h"
#include "Headers/Lexer.h"
#include "Headers/AliceTypes.h"

typedef enum EnumInstructionType {
    AdressDeclr,
    OperationDeclr,
    AliasDeclr,
    PositionDeclr
} NodeType;

typedef struct StructBlock
{
    Vector* Nodes;
} Block;

typedef struct StructAliceNumber 
{
    union
    {
        double f;
        int i;
    } Value;
    
} AliceNumber;

typedef struct StructConstant 
{
    AliceNumber* Value;
} Constant;

typedef enum EnumOprExpressionKind 
{
    CONSTANT,
    BINARY_OPERATION,
} OprExpressionKind;

typedef enum EnumBinaryOprKind 
{
    BIN_OPR_ADD,
    BIN_OPR_SUB,
    BIN_OPR_DIV,
    BIN_OPR_MUL
} BinaryOprKind;

typedef struct StructBinaryOpr 
{
    BinaryOprKind Operation;
    struct StructOprExpression* Left;
    struct StructOprExpression* Right;
} BinaryOpr;

typedef struct StructOprExpression 
{
    OprExpressionKind Kind;
    union
    {
        BinaryOpr BinaryExpr;
        Constant ConstantExpr;
    } Value;
    
} OprExpression;

typedef struct StructParserNode {
    NodeType Type;
    Usize Line;
    union 
    {
        struct StructFunctionArgument 
        {
            String* Name;
            AliceType Type;
        } FunctionArgument;
        struct StructFunction
        {
            String* Name;
            AliceType ReturnType;
        } Function;
        struct StructVariable
        {
            String* Name;
            AliceType Type;
        } Variable;
    } Data;
} ParserNode;

typedef struct StructParserState 
{
    Vector* Tokens;
    Usize Pointer;
    Alice_Token* Actual;
    Vector* Nodes;
} ParserState;

Vector* Parsenize(Alice_BigGirl* BigGirl, Vector* Tokens);
#endif