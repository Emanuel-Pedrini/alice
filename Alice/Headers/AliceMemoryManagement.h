#ifndef _AliceMemoryManagementH_
#define _AliceMemoryManagementH_

#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/ForwardInclude.h"
#include "External/Libraries.h"

typedef struct Struct__AliceAllocationChunk __AliceAllocationChunk;
typedef struct Struct__AliceAllocationArena __AliceAllocationArena;
typedef struct Struct__AliceAllocationArenaMark __AliceAllocationArenaMark;

#define __ALICE_MAIN_MARK_STACK_SIZE 128
#define __ALICE_ARENA_ALIGNMENT (8)

extern __AliceAllocationArena* __MainArena;

static Usize __ArenaAlignment(Usize N) { return ((N + __ALICE_ARENA_ALIGNMENT - 1) & ~(Usize)(__ALICE_ARENA_ALIGNMENT - 1)); }


struct  Struct__AliceAllocationChunk
{
    Usize Size;
    Usize Capacity;
    __AliceAllocationChunk* Next;
    char Data[];
};

struct Struct__AliceAllocationArena
{
    __AliceAllocationChunk* HeadChunk;
    Usize DefaultChunk;
};

struct Struct__AliceAllocationArenaMark
{
    __AliceAllocationChunk* Chunk;
    Usize Size;
};

__AliceAllocationArena* __NewAllocationArena(Usize Size);
__AliceAllocationArenaMark __NewAlAllocationMark(__AliceAllocationArena* __Arena);
__AliceAllocationChunk* __NewAlAllocationChunk();
__VoidPointer __AllocateAlArena(__AliceAllocationArena* __Arena, Usize __ObjectSize);
__VoidPointer __ReallocateAlArena(__AliceAllocationArena* __Arena, __VoidPointer __Object, Usize __NewObjectSize);
__Void __DeallocateAlArenaFromMark(__AliceAllocationArena* __Arena, __AliceAllocationArenaMark __Mark);
__Void __DeallocateAlArena(__AliceAllocationArena* __Arena);
__Void __MassDeallocateAlArena(__AliceAllocationArena* __Arena);
__VoidPointer __AlAllocate(Usize __ObjectSize);
__VoidPointer __AlReallocate(__VoidPointer __Object, Usize __OldObjectSize, Usize __ObjectSize);
__Void __AlDeallocate(__VoidPointer __Object);
__Void __AlFreeAll();


// Helpers
extern __AliceAllocationArenaMark __MainMarkStack[__ALICE_MAIN_MARK_STACK_SIZE];
extern Usize __MainMarkStackPointer;

#define __QMark {__MainMarkStack[__MainMarkStackPointer] = __NewAlAllocationMark(__MainArena);__MainMarkStackPointer++;}
#define __QUnmark {if (__MainMarkStackPointer > 0) {__DeallocateAlArenaFromMark(__MainArena, __MainMarkStack[__MainMarkStackPointer - 1]);__MainMarkStackPointer--;}}
#endif
