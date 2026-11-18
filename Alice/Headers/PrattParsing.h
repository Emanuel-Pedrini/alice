#ifndef __PRATT_PARSING__
#define __PRATT_PARSING__

#include "Headers/InterpreterTypes.h"
#include "External/Libraries.h"
#include "Headers/ArrayUtilities.h"
#include "Headers/Lexer.h"

typedef struct Struct__BinaryOperator BinaryOperator;
typedef struct Struct__UnaryOperator UnaryOperator;
typedef struct Struct__Expression Expression;
typedef struct Struct__ExpressionReader ExpressionReader;
typedef enum Enum__ExpressionType ExpressionType;

Usize BindingPower(const AliceTokenType Operator);
Bool CanBeUnary(const AliceTokenType Operator);

enum Enum__ExpressionType 
{
    LITERAL,
    UNARY,
    BINARY
};

String* ConversionToString__BinaryOperator(BinaryOperator* InQ);
String* ConversionToString__UnaryOperator(UnaryOperator* InQ);
String* ConversionToString__Expression(Expression* InQ);

struct Struct__BinaryOperator
{
    AliceToken* Operator;
    Expression* Left;
    Expression* Right;
    String* (*ConversionToString)(BinaryOperator*);
};

struct Struct__UnaryOperator
{
    AliceToken* Operator;
    Expression* Operand;
    String* (*ConversionToString)(UnaryOperator*);
};

typedef union Union__OperandValue
{
    AliceToken* Literal;
    BinaryOperator* Binary;
    UnaryOperator* Unary;
} OperandValue;

struct Struct__Expression
{
    ExpressionType Type;
    OperandValue Value;
    String* (*ConversionToString)(Expression*);
};

struct  Struct__ExpressionReader
{
    Usize Pointer;
    Vector* ConstExpression;
};

ExpressionReader* NewExpressionReader();
Expression* NewExpression(ExpressionType Type, OperandValue ConstOperand);

UnaryOperator* NewUnaryOperator(AliceToken* Operator, Expression* Operand);
BinaryOperator* NewBinaryOperator(AliceToken* Operator, Expression* Left, Expression* Right);

void NextExpressionReader(ExpressionReader* Reader);
AliceToken* CurrentExpressionReader(ExpressionReader* Reader);
AliceToken* ConsumeExpressionReader(ExpressionReader* Reader);
AliceToken* PeekExpressionReader(ExpressionReader* Reader, int Offset);

Expression* NullDenotation(ExpressionReader* Reader, AliceToken* Left);
Expression* LeftDenotation(ExpressionReader* Reader, Expression* Left, Expression* Right);
Expression* ParseExpression(ExpressionReader* Reader, Usize RightBindingPower);
#endif