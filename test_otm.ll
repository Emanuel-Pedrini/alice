; ModuleID = 'test.ll'
source_filename = "test.ll"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

; Function Attrs: mustprogress no__AlDeallocate norecurse nosync nounwind willreturn memory(none)
define i32 @summ(i32 %a, i32 %b) local_unnamed_addr #0 {
entry:
  %s = add i32 %b, %a
  ret i32 %s
}

; Function Attrs: mustprogress no__AlDeallocate norecurse nosync nounwind willreturn memory(none)
define noundef i32 @main() local_unnamed_addr #0 {
entry:
  ret i32 110
}

attributes #0 = { mustprogress no__AlDeallocate norecurse nosync nounwind willreturn memory(none) }
