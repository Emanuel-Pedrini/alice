#include "Headers/PrattParsing.h"

#define BASE_EXPRESSION_VALUE ((Usize)10)

Expression* NewExpression(ExpressionType Type, POperand ConstOperand)
{
    Expression* ExpressionN = malloc(sizeof(Expression));
    ExpressionN -> Type = Type;
    ExpressionN -> Value = ConstOperand;
    ExpressionN -> ConversionToString = ConversionToString__Expression;
    return ExpressionN;
}

UnaryOperator* NewUnaryOperator(Alice_Token* Operator, Expression* Operand)
{
    UnaryOperator* UnaryOp = malloc(sizeof(UnaryOperator));
    UnaryOp -> Operand = Operand;
    UnaryOp -> Operator = Operator;
    UnaryOp -> ConversionToString = ConversionToString__UnaryOperator;
    return UnaryOp;
}

BinaryOperator* NewBinaryOperator(Alice_Token* Operator, Expression* Left, Expression* Right)
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
    VECTOR(V, Alice_Token);
    ExpressionR -> ConstExpression = V; 
    ExpressionR -> Pointer = (Usize) 0;
    return ExpressionR;
}

String* ConversionToString__BinaryOperator(BinaryOperator* InQ)
{
    Expression* LeftExpr = InQ -> Left;
    Expression* RightExpr = InQ -> Right;

    String* S = FromStr("Binary <( ");
    AppendStr(S, ConversionToString__AliceToken(InQ -> Operator) -> Chars);
    AppendStr(S, " , ");
    String* LSE = LeftExpr -> ConversionToString(LeftExpr);
    String* RSE = RightExpr -> ConversionToString(RightExpr);
    AppendStr(S, LSE -> Chars);
    AppendStr(S, " , ");
    AppendStr(S, RSE -> Chars);
    AppendStr(S, " )>");
    return S;
}

String* ConversionToString__UnaryOperator(UnaryOperator* InQ)
{
    Expression* Expr = InQ -> Operand;
    String* S = FromStr("Unary <( ");
    AppendStr(S, ConversionToString__AliceToken(InQ -> Operator) -> Chars);
    AppendStr(S, ", ");
    String* SE = Expr -> ConversionToString(Expr);
    AppendStr(S, SE -> Chars);
    AppendStr(S, " )>");
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

String* ConversionToString__AliceToken(Alice_Token* InQ)
{
    return FromStr(InQ -> Literal);
}

void NextExpressionReader(ExpressionReader* Reader)
{
    Reader -> Pointer ++;
}

Alice_Token* CurrentExpressionReader(ExpressionReader* Reader)
{
    return (Alice_Token*)Get(Reader -> ConstExpression, Reader -> Pointer);
}

Alice_Token* ConsumeExpressionReader(ExpressionReader* Reader)
{
    Alice_Token* T = CurrentExpressionReader(Reader);
    NextExpressionReader(Reader);
    return T;
}

Alice_Token* PeekExpressionReader(ExpressionReader* Reader, int Offset)
{
    Usize Index = Reader -> Pointer + Offset;
    if (Index >= Reader -> ConstExpression -> size)
    {
        return NULL;
    }
    return (Alice_Token*)Get(Reader -> ConstExpression, Index);
}

Usize BindingPower(const Alice_TokenType Operator)
{
    switch (Operator)
    {
        case Tk_Minus:
            return BASE_EXPRESSION_VALUE * 1;
        case Tk_Plus:
            return BASE_EXPRESSION_VALUE * 1;
        case Tk_Asterisk:
            return BASE_EXPRESSION_VALUE * 2;
        case Tk_Div:
            return BASE_EXPRESSION_VALUE * 2;
        case Tk_LeftParenthesis:
            return BASE_EXPRESSION_VALUE * 0;
        case Tk_RightParenthesis:
            return BASE_EXPRESSION_VALUE * 0;
        case Tk_UpArrow:
            return BASE_EXPRESSION_VALUE * 4;
        default:
            return BASE_EXPRESSION_VALUE * 0;
    }
}
Bool CanBeUnary(const Alice_TokenType Operator)
{
    switch (Operator)
    {
        case Tk_Minus:
            return True;
        default:
            return False;
    }   
}

Expression* NullDenotation(ExpressionReader* Reader, Alice_Token* Left)
{
    Alice_TokenType T = Left -> Type;
    if (CanBeUnary(T))
    {
        Expression* ParsedExpression = ParseExpression(Reader, BindingPower(T));
        POperand P;
        UnaryOperator* UnOp = NewUnaryOperator(Left, ParsedExpression);
        P.Unary = UnOp;
        return NewExpression(UNARY, P);
    }
    else if (T == Tk_LeftParenthesis)
    {
        Expression* ParsedExpression = ParseExpression(Reader, BindingPower(T));
        Alice_Token* NextToken = ConsumeExpressionReader(Reader);
        if (NextToken -> Type == Tk_RightParenthesis)
        {
            return ParsedExpression;
        }
        return NULL;
    }
    POperand P;
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
    POperand P;
    P.Binary = BinOp;
    return NewExpression(BINARY, P);
}

Expression* ParseExpression(ExpressionReader* Reader, Usize RightBindingPower)
{
    Expression* LeftExpression = NullDenotation(
        Reader, 
        ConsumeExpressionReader(Reader)
    );
    Alice_Token* Next = PeekExpressionReader(Reader, 0);
    while (Next != NULL && BindingPower(Next -> Type) > RightBindingPower)
    {
        POperand P;
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