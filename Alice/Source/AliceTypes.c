#include "Headers/AliceTypes.h"
#include "External/Libraries.h"

#define __AlString_InitialCapacity 16
#define __AlVector_StandardSize 16
#define __AlVector_SizeEquation(x) (((x + 1) * 3))
#define __AlString_ConversionSize 64

__AliceString* __NewAlString()
{
    __AliceString* __AlString = malloc(sizeof(__AliceString));
    if(__AlString == NULL) return NULL;
    __AlString -> SSize = 0;
    __AlString -> SCapacity = __AlString_InitialCapacity;
    __AlString -> Chars = malloc(__AlString -> SCapacity);
    __AlString -> Chars[0] = '\0';
    return __AlString;
}

__AliceString* __AlStringFrom(const char* __Source)
{
    __AliceString* __AlString = __NewAlString();
    __AppendAlString(__AlString, __Source);
    return __AlString;
}

__AliceString* __ConcatenateAlString(Usize __Qtd, ...)
{
    va_list __Arguments;
    va_start(__Arguments, __Qtd);
    __AliceString* __AlString = __NewAlString();
    for (int __Interator = 0; __Interator < __Qtd; __Interator++)
    {
        __AliceString* __StringElement = va_arg(__Arguments, __AliceString*);
        __AppendAlString(__AlString, __StringElement -> Chars);
    }
    va_end(__Arguments);
    return __AlString;
}

__AliceString* __JoinAlString(const char* __Separator, Usize __Qtd, ...)
{
    va_list __Arguments;
    va_start(__Arguments, __Qtd);
    __AliceString* __AlString = __NewAlString();
    for (int __Interator = 0; __Interator < __Qtd; __Interator++)
    {
        if (__Interator == __Qtd - 1)
        {
            __AliceString* __StringElement = va_arg(__Arguments, __AliceString*);
            __AppendAlString(__AlString, __StringElement -> Chars);
        }
        else
        {
            __AliceString* __StringElement = va_arg(__Arguments, __AliceString*);
            __AppendAlString(__AlString, __StringElement -> Chars);
            __AppendAlString(__AlString, __Separator);
        }
    }
    va_end(__Arguments);
    return __AlString;
}

__AliceString* __IntToString(long int __N)
{
    char* __Buffer = malloc(__AlString_ConversionSize);
    snprintf(__Buffer, sizeof(__Buffer), "%d", __N);
    return __AlStringFrom(__Buffer);
}

__AliceString* __FloatToString(double __N)
{
    char* __Buffer = malloc(__AlString_ConversionSize);
    snprintf(__Buffer, sizeof(__Buffer), "%f", __N);
    return __AlStringFrom(__Buffer);
}

long int __StringToInt(__AliceString* __AlString, Usize __Base)
{
    char* __EndPointer;
    long int __Number = strtol(__AlString -> Chars, &__EndPointer, __Base);
    if (__AlString -> Chars != __EndPointer)
    {
        return __Number;
    }
    return 0;
}

double __StringToDouble(__AliceString* __AlString)
{
    char* __EndPointer;
    double __Number = strtod(__AlString -> Chars, &__EndPointer);
    if (__AlString -> Chars != __EndPointer)
    {
        return __Number;
    }
    return 0; 
}

__AliceVoid __AppendAlString(__AliceString* __AlString, const char* __Source)
{
    Usize __SourceLen = strlen(__Source);
    Usize __PretendedSize = (__AlString -> SSize + __SourceLen) + 1;
    if (__PretendedSize >= __AlString -> SCapacity)
    {
        Usize __NewCapacity = ((__PretendedSize * 2) + 1);
        char* __NewElements = realloc(__AlString -> Chars, __NewCapacity * sizeof(char));
        if (__NewElements == NULL)
        {
            free(__NewElements);
            return;
        }
        __AlString -> Chars = __NewElements;
        __AlString -> SCapacity = __NewCapacity;
    }
    for (int __Interator = 0; __Source[__Interator] != '\0'; __Interator++)
    {
        char __CharInterator = __Source[__Interator];
        __AlString -> Chars[__AlString -> SSize] = __CharInterator;
        __AlString -> SSize = __AlString -> SSize + 1;
    }
    __AlString -> Chars[__AlString -> SSize] = '\0';
}

__AliceVector* __NewAlVector(Usize __ElementSize)
{
    __AliceVector* __AlVector = malloc(sizeof(__AliceVector));
    __AlVector -> VElements = NULL;
    __AlVector -> ESize = __ElementSize;
    __AlVector -> VSize = 0;
    __AlVector -> VCapacity = 0;
    return __AlVector;
}

__AliceVector* __AlVectorWithCapacity(Usize __ElementSize, Usize __Capacity)
{
    __AliceVector* __AlVector = malloc(sizeof(__AliceVector));
    __AlVector -> VElements = calloc(__Capacity, __ElementSize);
    __AlVector -> ESize = __ElementSize;
    __AlVector -> VSize = 0;
    __AlVector -> VCapacity = __Capacity;
    return __AlVector;
}

__AliceVoid __ResizeAlVector(__AliceVector* __AlVector, Usize __NewSize)
{
    if (__AlVector -> VSize > __NewSize) return;
    __AliceVoidPtr __NewElements = realloc(__AlVector -> VElements, __NewSize * __AlVector -> ESize);
    if (__NewElements == NULL)
    {
        free(__NewElements);
        return;
    }
    __AlVector -> VElements = __NewElements;
    __AlVector -> VCapacity = __NewSize;
}

__AliceVoid __PushAlVector(__AliceVector* __AlVector, __AliceVoidPtr __Element)
{
    if (__AlVector -> VSize >= __AlVector -> VCapacity)
    {
        Usize __NewCapacity = __AlVector -> VSize <= 0 ? __AlVector_StandardSize : __AlVector_SizeEquation(__AlVector -> VCapacity);
        __AliceVoidPtr __NewElements = realloc(__AlVector -> VElements, __NewCapacity * __AlVector -> ESize);
        if (__NewElements == NULL)
        {
            free(__NewElements);
            return;
        }
        __AlVector -> VElements = __NewElements;
        __AlVector -> VCapacity = __NewCapacity;
    }
    char* __Destination = (char*)__AlVector -> VElements + __AlVector -> VSize * __AlVector -> ESize;
    memcpy(__Destination, __Element, __AlVector -> ESize);
    __AlVector -> VSize = __AlVector -> VSize + 1;
}

__AliceVoid __PopAlVector(__AliceVector* __AlVector)
{
    __AlVector -> VSize = __AlVector -> VSize - 1; 
}

__AliceVoidPtr __GetAlVector(__AliceVector* __AlVector, Usize __Index)
{
    return (char*)__AlVector -> VElements + __Index * __AlVector -> ESize;
}

__AliceVoid __MultiplePushAlVector(__AliceVector* __AlVector, Usize __Qtd, ...)
{
    va_list __Arguments;
    va_start(__Arguments, __Qtd);
    for (int __Interator = 0; __Interator < __Qtd; __Interator++)
    {
        __PushAlVector(__AlVector, va_arg(__Arguments, void*));
    }
    va_end(__Arguments);
}