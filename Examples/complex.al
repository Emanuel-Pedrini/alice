import "std", "random"
@initialize
func complex(argc : int'32, argv : char'8**) -> int'32:
    A : vec'[int'32]

    for i ; i < 1000 ; i++:
        A.append(rand::int32(1, 999))

    try thread::new() {

    }
    0
func count(vector : vec'[int'32]*) -> int'32:
    sum : int'32 = 0
    for i ; i < len(vector); i++:
        sum += vector[i]
    return sum