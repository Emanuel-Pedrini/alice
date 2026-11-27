#ifndef __PRATT_PARSING__
#define __PRATT_PARSING__

#include "External/Libraries.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/AliceTypes.h"
#include "Headers/Lexer.h"
#include "Headers/ForwardInclude.h"

typedef struct Struct__BinaryOperator BinaryOperator;
typedef struct Struct__UnaryOperator UnaryOperator;
typedef struct Struct__FunctionCallOperator AcessCallOperator;
typedef struct Struct__FieldAcessOperator FieldAcessOperator;

typedef struct Struct__ExpressionReader ExpressionReader;
typedef enum Enum__ExpressionType ExpressionType;

Usize BindingPower(ExpressionReader* Reader, const AliceTokenType Operator);
Bool CanBeUnary(const AliceTokenType Operator);

enum Enum__ExpressionType 
{
    LITERAL,
    UNARY,
    BINARY,
    CALL,
    NONE
};

struct Struct__BinaryOperator
{
    AliceToken* Operator;
    Expression* Left;
    Expression* Right;
};

struct Struct__UnaryOperator
{
    AliceToken* Operator;
    Expression* Operand;
};

struct Struct__FunctionCallOperator 
{
    Expression* Callee;
    __AliceVector* Arguments;
};

struct Struct__FieldAcessOperator
{
};

typedef union Union__OperandValue
{
    AliceToken* Literal;
    BinaryOperator* Binary;
    UnaryOperator* Unary;
    AcessCallOperator* Function;
    FieldAcessOperator* Field;
} OperandValue;

struct Struct__Expression
{
    ExpressionType Type;
    OperandValue Value;
    __AliceType* ResolvedType;
    LocalSymbol* Symbol;
};

struct  Struct__ExpressionReader
{
    Usize Pointer;
    AliceTokenType RightDelimiter;
    __AliceVector* ConstExpression;
};

ExpressionReader* NewExpressionReader();
Expression* NewExpression(ExpressionType Type, OperandValue ConstOperand);

UnaryOperator* NewUnaryOperator(AliceToken* Operator, Expression* Operand);
BinaryOperator* NewBinaryOperator(AliceToken* Operator, Expression* Left, Expression* Right);
AcessCallOperator* NewFunctionCallOperator(Expression* Callee, __AliceVector* Arguments);

void NextExpressionReader(ExpressionReader* Reader);
AliceToken* CurrentExpressionReader(ExpressionReader* Reader);
AliceToken* ConsumeExpressionReader(ExpressionReader* Reader);
AliceToken* PeekExpressionReader(ExpressionReader* Reader, int Offset);

Expression* NullDenotation(ExpressionReader* Reader, AliceToken* Left);
Expression* LeftDenotation(ExpressionReader* Reader, Expression* Left, Expression* Right);
Expression* ParseExpression(ExpressionReader* Reader, Usize RightBindingPower);

__AliceString* ConversionToString__BinaryOperator(BinaryOperator* InQ);
__AliceString* ConversionToString__UnaryOperator(UnaryOperator* InQ);
__AliceString* ConversionToString__Expression(Expression* InQ);
__AliceString* ConversionToString__FunctionCallOperator(AcessCallOperator* InQ);
#endif