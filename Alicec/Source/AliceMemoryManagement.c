#include "Headers/AliceMemoryManagement.h"

__AliceAllocationArena* __MainArena = NULL;
__AliceAllocationArenaMark __MainMarkStack[__ALICE_MAIN_MARK_STACK_SIZE];
Usize __MainMarkStackPointer = 0;

#define __MainAreaStandardSize (64 * 1024)

__AliceAllocationArena* __NewAllocationArena(Usize Size)
{
    __AliceAllocationArena* __Arena = malloc(sizeof(__AliceAllocationArena));
    __Arena -> DefaultChunk = Size;
    __Arena -> HeadChunk = NULL;
    return __Arena;
}

__AliceAllocationArenaMark __NewAlAllocationMark(__AliceAllocationArena* __Arena)
{
    __AliceAllocationChunk* __Head = __Arena -> HeadChunk;
    __AliceAllocationArenaMark __Mark = {__Head, __Head ? __Head -> Size : 0};
    return __Mark;
}

__AliceAllocationChunk* __NewAlAllocationChunk()
{

}

__VoidPointer __AllocateAlArena(__AliceAllocationArena* __Arena, Usize __ObjectSize)
{
    __ObjectSize = __ArenaAlignment(__ObjectSize);
    __AliceAllocationChunk* __Head = __Arena -> HeadChunk;
    if (__Head == NULL || __Head -> Size + __ObjectSize > __Head -> Capacity)
    {
        Usize __Capacity = __ObjectSize > __Arena -> DefaultChunk ? __ObjectSize : __Arena -> DefaultChunk;
        __AliceAllocationChunk* __NewHead = malloc(sizeof(__AliceAllocationChunk) + __Capacity);
        if (__NewHead == NULL) return NULL;
        __NewHead -> Next = __Head;
        __NewHead -> Size = 0;
        __NewHead -> Capacity = __Capacity;
        __Arena -> HeadChunk = __NewHead;
        __Head = __NewHead;
    }
    __VoidPointer __Pointer = __Head -> Data + __Head -> Size;
    __Head -> Size += __ObjectSize;
    return __Pointer;
}

__Void __DeallocateAlArena(__AliceAllocationArena* __Arena)
{
    __AliceAllocationArenaMark __HerculesMark = {NULL, 0};
    __DeallocateAlArenaFromMark(__Arena, __HerculesMark);
}

__Void __DeallocateAlArenaFromMark(__AliceAllocationArena* __Arena, __AliceAllocationArenaMark __Mark)
{
    while (__Arena -> HeadChunk != __Mark.Chunk)
    {
        __AliceAllocationChunk* __HydraHead = __Arena -> HeadChunk;
        __Arena -> HeadChunk = __HydraHead -> Next;
        free(__HydraHead);
    }
    if (__Arena -> HeadChunk) __Arena -> HeadChunk -> Size = __Mark.Size;
}

__Void __MassDeallocateAlArena(__AliceAllocationArena* __Arena)
{

}

__VoidPointer __AlAllocate(Usize __ObjectSize)
{
    if (__MainArena == NULL) __MainArena = __NewAllocationArena(__MainAreaStandardSize);
    __VoidPointer __Pointer = __AllocateAlArena(__MainArena, __ObjectSize);
    return __Pointer;
}

__VoidPointer __AlReallocate(__VoidPointer __Object, Usize __OldObjectSize, Usize __ObjectSize)
{
    if (__MainArena == NULL) __MainArena = __NewAllocationArena(__MainAreaStandardSize);
    __VoidPointer __NewPointer = __AllocateAlArena(__MainArena, __ObjectSize);
    if (__NewPointer != NULL && __Object != NULL) memcpy(__NewPointer, __Object, __OldObjectSize < __ObjectSize ? __OldObjectSize : __ObjectSize);
    return __NewPointer;
}


__Void __AlDeallocate(__VoidPointer __Object)
{

}

__Void __AlFreeAll()
{
    __DeallocateAlArena(__MainArena);
}