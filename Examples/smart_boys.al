static object Student:
    name : string

@initialize
function smart_boys(argc : int.32, argv : char.8**) -> int.32:
    /* Common pointer */
    def A : int'32 = 1
    def B : int'32* = &A
    def C : int'32* = B // Copy
    def D : int'32*
    move(D, B) // B is NULL, D is Equal &A

    /* Smart pointer */
    def X : int'32 = 2
    def Y : int'32<'a> = A

    return 0
