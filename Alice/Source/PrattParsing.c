#include "Headers/PrattParsing.h"

#define BASE_EXPRESSION_VALUE ((Usize)10)

Expression* NewExpression(ExpressionType Type, OperandValue ConstOperand)
{
    Expression* ExpressionN = malloc(sizeof(Expression));
    ExpressionN -> Type = Type;
    ExpressionN -> Value = ConstOperand;
    ExpressionN -> ConversionToString = ConversionToString__Expression;
    return ExpressionN;
}

UnaryOperator* NewUnaryOperator(AliceToken* Operator, Expression* Operand)
{
    UnaryOperator* UnaryOp = malloc(sizeof(UnaryOperator));
    UnaryOp -> Operand = Operand;
    UnaryOp -> Operator = Operator;
    UnaryOp -> ConversionToString = ConversionToString__UnaryOperator;
    return UnaryOp;
}

BinaryOperator* NewBinaryOperator(AliceToken* Operator, Expression* Left, Expression* Right)
{
    BinaryOperator* BinaryOp = malloc(sizeof(BinaryOperator));
    BinaryOp -> Left = Left;
    BinaryOp -> Right = Right;
    BinaryOp -> Operator = Operator;
    BinaryOp -> ConversionToString = ConversionToString__BinaryOperator;

    return BinaryOp;
}

ExpressionReader* NewExpressionReader()
{
    ExpressionReader* ExpressionR = malloc(sizeof(ExpressionReader));
    VECTOR(V, AliceToken);
    ExpressionR -> ConstExpression = V; 
    ExpressionR -> Pointer = (Usize) 0;
    return ExpressionR;
}

String* ConversionToString__BinaryOperator(BinaryOperator* InQ)
{
    Expression* LeftExpr = InQ -> Left;
    Expression* RightExpr = InQ -> Right;

    String* S = FromStr("Binary( ");
    AppendStr(S, (InQ -> Operator) -> ConversionToString(InQ -> Operator) -> Chars);
    AppendStr(S, " , ");
    String* LSE = LeftExpr -> ConversionToString(LeftExpr);
    String* RSE = RightExpr -> ConversionToString(RightExpr);
    AppendStr(S, LSE -> Chars);
    AppendStr(S, " , ");
    AppendStr(S, RSE -> Chars);
    AppendStr(S, " )");
    return S;
}

String* ConversionToString__UnaryOperator(UnaryOperator* InQ)
{
    Expression* Expr = InQ -> Operand;
    String* S = FromStr("Unary( ");
    AppendStr(S, (InQ -> Operator) -> ConversionToString(InQ -> Operator) -> Chars);
    AppendStr(S, " , ");
    String* SE = Expr -> ConversionToString(Expr);
    AppendStr(S, SE -> Chars);
    AppendStr(S, " )");
    return S;
}

String* ConversionToString__Expression(Expression* InQ)
{
    switch (InQ -> Type)
    {
        case UNARY:
        {
            UnaryOperator* UnOp = InQ -> Value.Unary;
            return UnOp -> ConversionToString(UnOp);
        }
        
        case BINARY:
        {
            BinaryOperator* BinOp = InQ -> Value.Binary;
            return BinOp -> ConversionToString(BinOp);
        }

        case LITERAL:
        {
            return ConversionToString__AliceToken(InQ -> Value.Literal);
        }
        default:
            return FromStr("Null");
    }
}

void NextExpressionReader(ExpressionReader* Reader)
{
    Reader -> Pointer ++;
}

AliceToken* CurrentExpressionReader(ExpressionReader* Reader)
{
    return (AliceToken*)GetByIndexVector(Reader -> ConstExpression, Reader -> Pointer);
}

AliceToken* ConsumeExpressionReader(ExpressionReader* Reader)
{
    AliceToken* T = CurrentExpressionReader(Reader);
    NextExpressionReader(Reader);
    return T;
}

AliceToken* PeekExpressionReader(ExpressionReader* Reader, int Offset)
{
    Usize Index = Reader -> Pointer + Offset;
    if (Index >= Reader -> ConstExpression -> size)
    {
        return NULL;
    }
    return (AliceToken*)GetByIndexVector(Reader -> ConstExpression, Index);
}

#define BindingOperator(T, P) case T: return BASE_EXPRESSION_VALUE * P;
Usize BindingPower(const AliceTokenType Operator)
{
    switch (Operator)
    {
        BindingOperator(Tk_LeftParenthesis, 0)
        BindingOperator(Tk_RightParenthesis, 0)
        BindingOperator(Tk_Minus, 1)
        BindingOperator(Tk_Plus, 1)
        BindingOperator(Tk_Asterisk, 2)
        BindingOperator(Tk_Div, 2)
        BindingOperator(Tk_Percentage, 2)
        BindingOperator(Tk_UpArrow, 4)
        default: return BASE_EXPRESSION_VALUE * 0;
    }
}

#define UnaryOperator(T) case T: return True;
Bool CanBeUnary(const AliceTokenType Operator)
{
    switch (Operator)
    {
        UnaryOperator(Tk_Minus)
        UnaryOperator(Tk_Plus)
        default: return False;
    }   
}

Expression* NullDenotation(ExpressionReader* Reader, AliceToken* Left)
{
    AliceTokenType T = Left -> Type;
    if (CanBeUnary(T))
    {
        Expression* ParsedExpression = ParseExpression(Reader, BindingPower(T));
        OperandValue P;
        UnaryOperator* UnOp = NewUnaryOperator(Left, ParsedExpression);
        P.Unary = UnOp;
        return NewExpression(UNARY, P);
    }
    else if (T == Tk_LeftParenthesis)
    {
        Expression* ParsedExpression = ParseExpression(Reader, BindingPower(T));
        AliceToken* NextToken = ConsumeExpressionReader(Reader);
        if (NextToken -> Type == Tk_RightParenthesis)
        {
            return ParsedExpression;
        }
        return NULL;
    }
    OperandValue P;
    P.Literal = Left;
    return NewExpression(LITERAL, P);
}

Expression* LeftDenotation(ExpressionReader* Reader, Expression* Left, Expression* Right)
{
    BinaryOperator* BinOp = NewBinaryOperator(
        Right -> Value.Literal, 
        Left, 
        ParseExpression(
            Reader, 
            BindingPower(
                Right -> Value.Literal -> Type
            ) - 1
        )
    );
    OperandValue P;
    P.Binary = BinOp;
    return NewExpression(BINARY, P);
}

Expression* ParseExpression(ExpressionReader* Reader, Usize RightBindingPower)
{
    Expression* LeftExpression = NullDenotation(
        Reader, 
        ConsumeExpressionReader(Reader)
    );
    AliceToken* Next = PeekExpressionReader(Reader, 0);
    while (Next != NULL && BindingPower(Next -> Type) > RightBindingPower)
    {
        OperandValue P;
        P.Literal = ConsumeExpressionReader(Reader);
        Expression* Expr = NewExpression(LITERAL, P);
        LeftExpression = LeftDenotation(
            Reader,
            LeftExpression,
            Expr
        );
        Next = PeekExpressionReader(Reader, 0);
    }
    return LeftExpression;
}