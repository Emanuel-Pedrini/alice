#include "Headers/ArrayUtilities.h"
#include "External/Libraries.h"
#include "Headers/InterpreterTypes.h"

void Add(Vector* Vector, const void* Element) 
{
    if (Vector -> size >=  Vector -> capacity) 
    {
        Usize newCapacity = Vector -> size == 0 ? 8 : Vector -> capacity * 2;
        void* newElements =  realloc(Vector -> elements, newCapacity * Vector -> elementSize);
        if (newElements == NULL)
        {
            return;
        }
        Vector -> elements = newElements;
        Vector -> capacity = newCapacity;
    }
    
    char* destination = (char*)Vector -> elements + Vector -> size * Vector -> elementSize;
    memcpy(
        destination,
        Element,
        Vector -> elementSize
    );

    Vector -> size++;
}

void New(Vector* Vector, Usize ElementSize) 
{
    Vector -> elements = NULL;
    Vector -> capacity = 0;
    Vector -> size = 0;
    Vector -> elementSize = ElementSize;
}

void* Get(Vector* Vector, Usize Index) 
{
    if (Index >=  Vector -> size) {
        return NULL;
    }
    return (char*)Vector -> elements + Index * Vector -> elementSize;
}

void Free(Vector* Vector)
{
    free(Vector -> elements);
    Vector -> elements = NULL;
    Vector -> capacity = 0;
    Vector -> size = 0;
}

Usize Len(Vector* Vector) 
{
    return Vector -> size;
}