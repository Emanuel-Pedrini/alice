@initialize
function burn_baby(argc : int'32, argv : char'8**) -> int'32:
    def A : int'32 = 42
    def B : float'64 = <float'64> A 
    def C : int'32 = sum(A, B)
    if C >= A:
        return 1
        if C <= A:
            return 2
        else:
            return 3
    return 0

function sum(a : int'32, b : int'32) -> int'32:
    return (a + b)
    if
    else