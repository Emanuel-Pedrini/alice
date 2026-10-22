#ifndef _ArrayUtilitiesH_
#define _ArrayUtilitiesH_
#include "Headers/InterpreterTypes.h"

typedef struct {
    Usize size;
    Usize elementSize;
    Usize capacity;
    void* elements;
} Vector;

void Add(Vector* Vector, const void* Element);
void New(Vector* Vector, Usize ElementSize);
void* Get(Vector* Vector, Usize Index);
void Free(Vector* Vector);
Usize Len(Vector* Vector);

#endif