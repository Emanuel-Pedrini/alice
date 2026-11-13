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

Usize BindingPower(const Alice_TokenType Operator);
Bool CanBeUnary(const Alice_TokenType Operator);

enum Enum__ExpressionType 
{
    LITERAL,
    UNARY,
    BINARY
};

String* ConversionToString__BinaryOperator(BinaryOperator* InQ);
String* ConversionToString__UnaryOperator(UnaryOperator* InQ);
String* ConversionToString__Expression(Expression* InQ);
String* ConversionToString__AliceToken(Alice_Token* InQ);

struct Struct__BinaryOperator
{
    Alice_Token* Operator;
    Expression* Left;
    Expression* Right;
    String* (*ConversionToString)(BinaryOperator*);
};

struct Struct__UnaryOperator
{
    Alice_Token* Operator;
    Expression* Operand;
    String* (*ConversionToString)(UnaryOperator*);
};

typedef union Union__POperand
{
    Alice_Token* Literal;
    BinaryOperator* Binary;
    UnaryOperator* Unary;
} POperand;

struct Struct__Expression
{
    ExpressionType Type;
    POperand Value;
    String* (*ConversionToString)(Expression*);
};

struct  Struct__ExpressionReader
{
    Usize Pointer;
    Vector* ConstExpression;
};

ExpressionReader* NewExpressionReader();
Expression* NewExpression(ExpressionType Type, POperand ConstOperand);

UnaryOperator* NewUnaryOperator(Alice_Token* Operator, Expression* Operand);
BinaryOperator* NewBinaryOperator(Alice_Token* Operator, Expression* Left, Expression* Right);

void NextExpressionReader(ExpressionReader* Reader);
Alice_Token* CurrentExpressionReader(ExpressionReader* Reader);
Alice_Token* ConsumeExpressionReader(ExpressionReader* Reader);
Alice_Token* PeekExpressionReader(ExpressionReader* Reader, int Offset);

Expression* NullDenotation(ExpressionReader* Reader, Alice_Token* Left);
Expression* LeftDenotation(ExpressionReader* Reader, Expression* Left, Expression* Right);
Expression* ParseExpression(ExpressionReader* Reader, Usize RightBindingPower);
#endif