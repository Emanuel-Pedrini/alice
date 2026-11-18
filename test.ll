target triple = "x86_64-w64-windows-gnu"
define i32 @summ(i32 %a, i32 %b) {
entry:
    %s = add i32 %a, %b
    ret i32 %s
}

define i32 @main() {
    entry:
    %r = call i32 @summ(i32 33, i32 77)
    ret i32 %r
}