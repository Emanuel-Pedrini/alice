fn main() i32:
    a : i32 = 89
    b : i32 = 68*((-7-90)/(5+a))
    c : i32 = 6478
    d : Array<i32, 3> = Array.new(a, b, c)
    j : Vector<String> = Vector.new()
    k : i16 = j.RemoveLast()
    &MyObject Object = MyObject.new()
    Object!

struct MyObject:
    fn static new() &MyObject:
        &MyObject Fobj = MemoryAlloc(MyObject)
        return Fobj


