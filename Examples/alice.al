y : i32 = 8
x : i32 = 68 * ( ( -7 - 90 ) / ( 5 + y ) )

( * C ( / ( - C C ) ( + C V ) ) )
( * 68 ( / ( - -7 90 ) ( + 5 y ) ) )


BinOp( MUL, 68, BinOp( DIVISION, BinOp( MINUS, UnaryOp(NEG, 7), 90 ), BinOp( PLUS, 5, Var(y) ) ) )
