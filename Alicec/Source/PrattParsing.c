#include "Headers/PrattParsing.h"

#define BASE_EXPRESSION_VALUE ((Usize)10)

Expression* NewExpression(ExpressionType Type, OperandValue ConstOperand)
{
    Expression* ExpressionN = __AlAllocate(sizeof(Expression));
    ExpressionN -> Type = Type;
    ExpressionN -> Value = ConstOperand;
    return ExpressionN;
}

AcessCallOperator* NewFunctionCallOperator(Expression* Callee, __AliceVector* Arguments)
{
    AcessCallOperator* OperatorF = __AlAllocate(sizeof(AcessCallOperator));
    OperatorF -> Callee = Callee;
    OperatorF -> Arguments = Arguments;
    return OperatorF; 
}

UnaryOperator* NewUnaryOperator(AliceToken* Operator, Expression* Operand)
{
    UnaryOperator* UnaryOp = __AlAllocate(sizeof(UnaryOperator));
    UnaryOp -> Operand = Operand;
    UnaryOp -> Operator = Operator;
    return UnaryOp;
}

BinaryOperator* NewBinaryOperator(AliceToken* Operator, Expression* Left, Expression* Right)
{
    BinaryOperator* BinaryOp = __AlAllocate(sizeof(BinaryOperator));
    BinaryOp -> Left = Left;
    BinaryOp -> Right = Right;
    BinaryOp -> Operator = Operator;
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

__AliceString* ConversionToString__FunctionCallOperator(AcessCallOperator* InQ)
{
    __AliceString* Result = __AlStringFrom("");
    __AppendAlString(Result, "AcessObject( ");
    Expression* Expr = InQ -> Callee;
    __AppendAlString(Result, ConversionToString__Expression(Expr) -> Chars);
    __AliceVector* Args = InQ -> Arguments;
    
    __AliceString* __Result = __AlStringFrom(", [ ");
    __ForIn(Args, __I, 0)
    {
        Expression* __Element = *((Expression**)__GetAlVector(Args, __I));
        __AliceString* __ElementString = ConversionToString__Expression(Expr);
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
    __AppendAlString(S, ConversionToString__AliceToken(InQ -> Operator) -> Chars);
    __AppendAlString(S, " , ");
    __AliceString* LSE = ConversionToString__Expression(LeftExpr);
    __AliceString* RSE = ConversionToString__Expression(RightExpr);
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
    __AppendAlString(S, ConversionToString__AliceToken(InQ -> Operator) -> Chars);
    __AppendAlString(S, " , ");
    __AliceString* SE = ConversionToString__Expression(Expr);
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
            return ConversionToString__UnaryOperator(UnOp);
        }
        
        case BINARY:
        {
            BinaryOperator* BinOp = InQ -> Value.Binary;
            return ConversionToString__BinaryOperator(BinOp);
        }

        case LITERAL:
        {
            return ConversionToString__AliceToken(InQ -> Value.Literal);
        }
        case CALL:
        {
            AcessCallOperator* AcessOp = InQ -> Value.Function;
            return ConversionToString__FunctionCallOperator(AcessOp);
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
Usize BindingPower(ExpressionReader* Reader, const AliceTokenType Operator)
{
    if (Operator == Reader -> RightDelimiter) return BASE_EXPRESSION_VALUE * 0;
    switch (Operator)
    {
        BindingOperator(ALICETK_LEFTPARENTHESIS, 128)
        BindingOperator(ALICETK_LEFTKEYS, 128)
        BindingOperator(ALICETK_LEFTBRACE, 128)
        BindingOperator(ALICETK_RIGHTBRACE, 0)
        BindingOperator(ALICETK_RIGHTPARENTHESIS, 0)

        // Boolean operators
        BindingOperator(ALICETK_LEFTARROW, 8)
        BindingOperator(ALICETK_RIGHTARROW, 8)
        BindingOperator(ALICETK_GREATEREQUAL, 8)
        BindingOperator(ALICETK_LESSEQUAL, 8)
        BindingOperator(ALICETK_EQUALEQUAL, 4)
        BindingOperator(ALICETK_NOTEQUAL, 4)
        BindingOperator(ALICETK_AND, 2)
        BindingOperator(ALICETK_OR, 1)

        BindingOperator(ALICETK_MINUS, 16)
        BindingOperator(ALICETK_PLUS, 16)
        BindingOperator(ALICETK_ASTERISK, 32)
        BindingOperator(ALICETK_DIV, 32)
        BindingOperator(ALICETK_PERCENT, 64)
        BindingOperator(ALICETK_UPARROW, 64)
        BindingOperator(ALICETK_POINT, 128)
        default: return BASE_EXPRESSION_VALUE * 0;
    }
}

#define UnaryOperator(T) case T: return True;
Bool CanBeUnary(const AliceTokenType Operator)
{
    switch (Operator)
    {
        UnaryOperator(ALICETK_MINUS)
        UnaryOperator(ALICETK_PLUS)
        UnaryOperator(ALICETK_NOT)
        UnaryOperator(ALICETK_EXPLICITCAST)
        UnaryOperator(ALICETK_COWARDCAST)
        default: return False;
    }   
}

Bool CanBeAcess(const AliceTokenType Operator)
{
    switch (Operator)
    {
        UnaryOperator(ALICETK_LEFTPARENTHESIS)
        UnaryOperator(ALICETK_LEFTBRACE)
        UnaryOperator(ALICETK_LEFTKEYS)
        default: return False;
    }   
}

Bool AcessCloser(const AliceTokenType Opener)
{
    switch (Opener)
    {
        case ALICETK_LEFTPARENTHESIS: return ALICETK_RIGHTPARENTHESIS;
        case ALICETK_LEFTBRACE: return ALICETK_RIGHTBRACE;
        case ALICETK_LEFTKEYS: return ALICETK_RIGHTKEYS;
        default: return ALICETK_ILLEGAL;
    }
}

Expression* CallDenotation(ExpressionReader* Reader, Expression* Callee)
{
    __AliceVector* Arguments = __NewAlVector(sizeof(Expression*));
    AliceToken* Next = PeekExpressionReader(Reader, 0);
    AliceToken* Opener = PeekExpressionReader(Reader, -1);
    AliceTokenType Closer = AcessCloser(Opener -> Type);
    if (Next != NULL && Next -> Type != Closer)
    {
        while (True)
        {
            Expression* Argument = ParseExpression(Reader, 0);
            __PushAlVector(Arguments, &Argument);
            Next = PeekExpressionReader(Reader, 0);
            if (Next != NULL && Next -> Type == ALICETK_COMMA)
            {
                ConsumeExpressionReader(Reader);
                continue;
            }
            break;
        }
    }
    AliceToken* Encloser = PeekExpressionReader(Reader, 0);
    if (Encloser == NULL || Encloser -> Type != Closer) return NULL;
    ConsumeExpressionReader(Reader);
    OperandValue P;
    AcessCallOperator* AcessOp = NewFunctionCallOperator(Callee, Arguments);
    P.Function = AcessOp;
    return NewExpression(CALL, P);
}

Expression* NullDenotation(ExpressionReader* Reader, AliceToken* Left)
{
    AliceTokenType T = Left -> Type;
    if (CanBeUnary(T))
    {
        Expression* ParsedExpression = ParseExpression(Reader, BindingPower(Reader, T));
        OperandValue P;
        UnaryOperator* UnOp = NewUnaryOperator(Left, ParsedExpression);
        P.Unary = UnOp;
        return NewExpression(UNARY, P);
    }
    else if (T == ALICETK_LEFTPARENTHESIS)
    {
        Expression* ParsedExpression = ParseExpression(Reader, 0);
        AliceToken* NextToken = ConsumeExpressionReader(Reader);
        if (NextToken -> Type == ALICETK_RIGHTPARENTHESIS)
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
    Usize BindPower = BindingPower(Reader, BindType);
    if (BindType == ALICETK_UPARROW) BindPower--;
    if (CanBeAcess(BindType)) return CallDenotation(Reader, Left);
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
    while (Next != NULL && BindingPower(Reader, Next -> Type) > RightBindingPower)
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