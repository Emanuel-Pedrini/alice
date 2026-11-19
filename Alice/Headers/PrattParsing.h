#ifndef __PRATT_PARSING__
#define __PRATT_PARSING__

#include "External/Libraries.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/Lexer.h"

typedef struct Struct__BinaryOperator BinaryOperator;
typedef struct Struct__UnaryOperator UnaryOperator;
typedef struct Struct__FunctionCallOperator FunctionCallOperator;
typedef struct Struct__FieldAcessOperator FieldAcessOperator;

typedef struct Struct__Expression Expression;
typedef struct Struct__ExpressionReader ExpressionReader;
typedef enum Enum__ExpressionType ExpressionType;

Usize BindingPower(const AliceTokenType Operator);
Bool CanBeUnary(const AliceTokenType Operator);

enum Enum__ExpressionType 
{
    LITERAL,
    UNARY,
    BINARY,
    CALL,
    FIELD
};

struct Struct__BinaryOperator
{
    AliceToken* Operator;
    Expression* Left;
    Expression* Right;
    __AliceString* (*ConversionToString)(BinaryOperator*);
};

struct Struct__UnaryOperator
{
    AliceToken* Operator;
    Expression* Operand;
    __AliceString* (*ConversionToString)(UnaryOperator*);
};

struct Struct__FunctionCallOperator 
{
    Expression* Callee;
    __AliceVector* Arguments;
    __AliceString* (*ConversionToString)(FunctionCallOperator*);
};

struct Struct__FieldAcessOperator
{
    __AliceString* (*ConversionToString)(FieldAcessOperator*);
};

typedef union Union__OperandValue
{
    AliceToken* Literal;
    BinaryOperator* Binary;
    UnaryOperator* Unary;
    FunctionCallOperator* Function;
    FieldAcessOperator* Field;
} OperandValue;

struct Struct__Expression
{
    ExpressionType Type;
    OperandValue Value;
    __AliceString* (*ConversionToString)(Expression*);
};

struct  Struct__ExpressionReader
{
    Usize Pointer;
    __AliceVector* ConstExpression;
};

ExpressionReader* NewExpressionReader();
Expression* NewExpression(ExpressionType Type, OperandValue ConstOperand);

UnaryOperator* NewUnaryOperator(AliceToken* Operator, Expression* Operand);
BinaryOperator* NewBinaryOperator(AliceToken* Operator, Expression* Left, Expression* Right);
FunctionCallOperator* NewFunctionCallOperator(Expression* Callee, __AliceVector* Arguments);

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
__AliceString* ConversionToString__FunctionCallOperator(FunctionCallOperator* InQ);
#endif