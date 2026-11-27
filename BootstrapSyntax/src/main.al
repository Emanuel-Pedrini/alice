// For a minimum syntax that i'll support the bootstrap we must have at least
// Structs, Functions, Recursion, Unions, Enums, String, Dynamic memory
//

fn main() -> i32: // Functions, OK.
    FileF* FileToOpen = openFile("alice.al", "w")
    String Text = FileToOpen.IntoString()
    ~ FileToOpen

    a : & i32 = loc i32, 0 
    b : & AliceToken = loc AliceToken
    c : & Null = reloc 

    p : i32 = 0
    while True:
        current : Char = Text[p]
        p++ 
    return 0

define AliceTokenStream: // Structs, OK.
    pointer : u32
    tokens : Dynamic<AliceToken>

define AliceToken: 
    literal : String // String, OK.
    start : u32
    length : u32
    kind : AliceTokenKind

define enum AliceTokenKind: // Enums, OK.
    ALICETK_ILLEGAL,
    ALICETK_IDENTIFIER,
    ALICETK_NUMBER,
    ALICETK_FUNCTION

define ParserNodeBody:
    statements : Dynamic<ParserNode>

define ParserNodeExpr:
    pass

define ParserNodeFunc:
    name : String
    body : ParserNodeBody

define enum ParserNode: // Unions, OK.
    EXPRESSION(ParserNodeExpr),
    FUNCTION(ParserNodeFunc),
    BODY(ParserNodeBody)


fn recursion(n : i32) -> i32:
    if n == 0:
        return 1
    return recursion(n - 1)