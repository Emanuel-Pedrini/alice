@init
fn matrixes() -> int'32:
    let MTX : int'32[4][4]
    
    for x = 0; x < len(MTX) ; x++:
        for y = 0; y < MTX[x]; y++:
            MTX[y] = x + y
    /*
        0, 1, 2, 3
        1, 2, 3, 4
        2, 3, 4, 5
        3, 4, 5, 6
    */
    
    for x = 0; x < len(MTX) ; x++:
        for y = 0; y < MTX[x]; y++:
            let act : int'32 = MTX[y]
            match act:
                1, 2, 3:
                    println!("{int}!", act)
                4:
                    println!("{str}", "QUATRO!")
                _:
                    ignore
    
    for x in MTX:
        for y in x:
            println!("{int}", y)

    return 0
