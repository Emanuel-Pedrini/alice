@initialize
function loops(argc : int'32, argv : char'8**) -> int'32:
    def A : [int'32 , 100] = {679}
    def B : [int'32, 100] = {39765}
    def C : [float'64, 100] = {895.47}
    def D : [int'32, 100] = {0}

    for x : usize'64 = 0; x < len(D); x++:
        D[x] = A[x] + B[x]

    def x : usize'64 = 0 
    while x < len(D):
        D[x] = <int'32> C[x]
        x++
    
    def y : usize'64 = 27
    loop:
        if y == 0:
            break
        elif y % 2 == 0:
            y = y / 2
        else:
            y = (y * 3) + 1
