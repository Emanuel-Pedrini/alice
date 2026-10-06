@init
fn burn_baby(argc : int'32, argv : char'8**) -> int'32:
    A : int'32
    A = 42
    B : float'64 = <float'64> A /* EXPLICIT CAST */
    C : int'32 = sum(A, B)
    D : int'32 = sum()

    if C >= A or C < D:
        return 1
    return 0 

fn sum(a : int'32 = 0, b : int'32 = 0) -> int'32:
    return (a + b)