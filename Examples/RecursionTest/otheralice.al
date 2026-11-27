fn main() -> i32:
    my_stt : Static<i32, ?> = S'{0 .. 999}
    my_sum : i32 = sum_static(s)
    ~ my_stt
    return d

fn sum_static(s : Static<i32>) -> i32:
    sum : i32 = 0
    for i in s:
        sum += i
    return sum