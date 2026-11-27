fn main() -> i32:
    a : i32 = 3
    b : i32 = 6 * a + 5
    return b + 9

/* 
AIR
    MODULE
        FUNCTION "main"
            %_a0 = alloc i32, al 4
            %_a1 = alloc i32, al 4
            BLOCK "entry"
                %_t0 = add i32 0, 3
                store i32 %_t0, ptr %_a0, al 4
                
                %_t1 = load i32 ptr %_a0, al 4
                %_t2 = mul i32 %_t1, 6
                %_t3 = add i32, %_t2, 5
                store i32 %_t3, ptr %_a1, al 4

                %_t4 = load i32 ptr %_a1, al 4
                %_t5 = add i32 %_t4, 9
                ret i32 %_t5
*/