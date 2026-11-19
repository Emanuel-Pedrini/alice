#include "Headers/PrattParsing.h"

#define BASE_EXPRESSION_VALUE ((Usize)10)

Expression* NewExpression(ExpressionType Type, OperandValue ConstOperand)
{
    Expression* ExpressionN = __AlAllocate(sizeof(Expression));
    ExpressionN -> Type = Type;
    ExpressionN -> Value = ConstOperand;
    ExpressionN -> ConversionToString = ConversionToString__Expression;
    return ExpressionN;
}

FunctionCallOperator* NewFunctionCallOperator(Expression* Callee, __AliceVector* Arguments)
{
    FunctionCallOperator* OperatorF = __AlAllocate(sizeof(FunctionCallOperator));
    OperatorF -> Callee = Callee;
    OperatorF -> Arguments = Arguments;
    OperatorF -> ConversionToString = ConversionToString__FunctionCallOperator;
    return OperatorF; 
}

UnaryOperator* NewUnaryOperator(AliceToken* Operator, Expression* Operand)
{
    UnaryOperator* UnaryOp = __AlAllocate(sizeof(UnaryOperator));
    UnaryOp -> Operand = Operand;
    UnaryOp -> Operator = Operator;
    UnaryOp -> ConversionToString = ConversionToString__UnaryOperator;
    return UnaryOp;
}

BinaryOperator* NewBinaryOperator(AliceToken* Operator, Expression* Left, Expression* Right)
{
    BinaryOperator* BinaryOp = __AlAllocate(sizeof(BinaryOperator));
    BinaryOp -> Left = Left;
    BinaryOp -> Right = Right;
    BinaryOp -> Operator = Operator;
    BinaryOp -> ConversionToString = ConversionToString__BinaryOperator;

    return BinaryOp;
}

ExpressionReader* NewExpressionReader()
{
    ExpressionReader* ExpressionR = __AlAllocate(sizeof(ExpressionReader));
    __AliceVector* V = __NewAlVector(sizeof(AliceToken));
    ExpressionR -> ConstExpression = V; 
    ExpressionR -> Pointer = (Usize) 0;
    return ExpressionR;
}

__AliceString* ConversionToString__FunctionCallOperator(FunctionCallOperator* InQ)
{
    __AliceString* Result = __AlStringFrom("");
    __AppendAlString(Result, "Func( ");
    Expression* Expr = InQ -> Callee;
    __AppendAlString(Result, Expr -> ConversionToString(Expr) -> Chars);
    __AliceVector* Args = InQ -> Arguments;
    
    __AliceString* __Result = __AlStringFrom(", [ ");
    __ForIn(Args, __I, 0)
    {
        Expression* __Element = *((Expression**)__GetAlVector(Args, __I));
        __AliceString* __ElementString = __Element -> ConversionToString(__Element);
        __AppendAlString(__Result, __ElementString -> Chars);
        if ((__I + 1 <  Args -> VSize)) __AppendAlString(__Result, " , ");
    }
    __AppendAlString(__Result, " ]");
    __AppendAlString(Result, __Result -> Chars);

    __AppendAlString(Result, " )");
    return Result;
}

__AliceString* ConversionToString__BinaryOperator(BinaryOperator* InQ)
{
    Expression* LeftExpr = InQ -> Left;
    Expression* RightExpr = InQ -> Right;

    __AliceString* S = __AlStringFrom("Binary( ");
    __AppendAlString(S, (InQ -> Operator) -> ConversionToString(InQ -> Operator) -> Chars);
    __AppendAlString(S, " , ");
    __AliceString* LSE = LeftExpr -> ConversionToString(LeftExpr);
    __AliceString* RSE = RightExpr -> ConversionToString(RightExpr);
    __AppendAlString(S, LSE -> Chars);
    __AppendAlString(S, " , ");
    __AppendAlString(S, RSE -> Chars);
    __AppendAlString(S, " )");
    return S;
}

__AliceString* ConversionToString__UnaryOperator(UnaryOperator* InQ)
{
    Expression* Expr = InQ -> Operand;
    __AliceString* S = __AlStringFrom("Unary( ");
    __AppendAlString(S, (InQ -> Operator) -> ConversionToString(InQ -> Operator) -> Chars);
    __AppendAlString(S, " , ");
    __AliceString* SE = Expr -> ConversionToString(Expr);
    __AppendAlString(S, SE -> Chars);
    __AppendAlString(S, " )");
    return S;
}

__AliceString* ConversionToString__Expression(Expression* InQ)
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
        case CALL:
        {
            FunctionCallOperator* FuncOp = InQ -> Value.Function;
            return FuncOp -> ConversionToString(FuncOp);
        }
        default:
            return __AlStringFrom("Null");
    }
}

void NextExpressionReader(ExpressionReader* Reader)
{
    Reader -> Pointer ++;
}

AliceToken* CurrentExpressionReader(ExpressionReader* Reader)
{
    return (AliceToken*)__GetAlVector(Reader -> ConstExpression, Reader -> Pointer);
}

AliceToken* ConsumeExpressionReader(ExpressionReader* Reader)
{
    AliceToken* T = PeekExpressionReader(Reader, 0);
    NextExpressionReader(Reader);
    return T;
}

AliceToken* PeekExpressionReader(ExpressionReader* Reader, int Offset)
{
    Usize Index = Reader -> Pointer + Offset;
    if (Index >= Reader -> ConstExpression -> VSize)
    {
        return NULL;
    }
    return (AliceToken*)__GetAlVector(Reader -> ConstExpression, Index);
}

#define BindingOperator(T, P) case T: return BASE_EXPRESSION_VALUE * P;
Usize BindingPower(const AliceTokenType Operator)
{
    switch (Operator)
    {
        BindingOperator(Tk_LeftParenthesis, 128)
        BindingOperator(Tk_RightParenthesis, 0)

        // Boolean operators
        BindingOperator(Tk_LeftArrow, 8)
        BindingOperator(Tk_RightArrow, 8)
        BindingOperator(Tk_GreaterEqual, 8)
        BindingOperator(Tk_LessEqual, 8)
        BindingOperator(Tk_EqualEqual, 4)
        BindingOperator(Tk_NotEqual, 4)
        BindingOperator(Tk_And, 2)
        BindingOperator(Tk_Or, 1)

        BindingOperator(Tk_Minus, 16)
        BindingOperator(Tk_Plus, 16)
        BindingOperator(Tk_Asterisk, 32)
        BindingOperator(Tk_Div, 32)
        BindingOperator(Tk_Percentage, 64)
        BindingOperator(Tk_UpArrow, 64)
        BindingOperator(Tk_Point, 128)
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
        UnaryOperator(Tk_Not)
        default: return False;
    }   
}
Expression* CallDenotation(ExpressionReader* Reader, Expression* Callee)
{
    __AliceVector* Arguments = __NewAlVector(sizeof(Expression*));
    AliceToken* Next = PeekExpressionReader(Reader, 0);
    if (Next != NULL && Next -> Type != Tk_RightParenthesis)
    {
        while (True)
        {
            Expression* Argument = ParseExpression(Reader, 0);
            __PushAlVector(Arguments, &Argument);
            Next = PeekExpressionReader(Reader, 0);
            if (Next != NULL && Next -> Type == Tk_Comma)
            {
                ConsumeExpressionReader(Reader);
                continue;
            }
            break;
        }
    }
    AliceToken* Encloser = PeekExpressionReader(Reader, 0);
    if (Encloser == NULL || Encloser -> Type != Tk_RightParenthesis) return NULL;
    ConsumeExpressionReader(Reader);
    OperandValue P;
    FunctionCallOperator* FuncOp = NewFunctionCallOperator(Callee, Arguments);
    P.Function = FuncOp;
    return NewExpression(CALL, P);
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
        Expression* ParsedExpression = ParseExpression(Reader, 0);
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
    AliceTokenType BindType = Right -> Value.Literal -> Type;
    Usize BindPower = BindingPower(BindType);
    if (BindType == Tk_UpArrow) BindPower--;
    if (BindType == Tk_LeftParenthesis) return CallDenotation(Reader, Left);
    BinaryOperator* BinOp = NewBinaryOperator(
        Right -> Value.Literal, 
        Left, 
        ParseExpression(
            Reader,
            BindPower
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