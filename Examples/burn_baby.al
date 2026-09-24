@initialize
func burn_baby(argc : int'32, argv : char'8**) -> int'32:
    A : int'32 = 42
    B : float'64 = <float'64> A // Explicit cast
    C : int'32 = sum(A, B)
    if C >= A:
        return 1
    return 0

func sum(a : int'32 = 0, b : int'32 = 0) -> int'32:
    return (a + b)