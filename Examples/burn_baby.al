@initialize
function burn_baby(argc : int'32, argv : char'8**) -> int'32:
    def A : int'32 = 42
    def B : int'32 = 1945
    def C : float'64 = <float'64> A 
    def D : int'32 = sum(A, B)
    return 0

function sum(a : int'32, b : int'32) -> int'32:
    return (a + b)
