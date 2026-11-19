#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/AliceMemoryManagement.h"
#include "External/Libraries.h"
#include "Headers/Lexer.h"
#include "Headers/PrattParsing.h"

#define __AlString_InitialCapacity 16
#define __AlVector_StandardSize 16
#define __AlVector_SizeEquation(x) (((x + 1) * 3))
#define __AlString_ConversionSize 64

__Void __DropAlString(void* __AlString)
{

}

__AliceString* __NewAlString()
{
    __AliceString* __AlString = __AlAllocate(sizeof(__AliceString));
    if(__AlString == NULL) return NULL;
    __AlString -> SSize = 0;
    __AlString -> SCapacity = __AlString_InitialCapacity;
    __AlString -> Chars = __AlAllocate(__AlString -> SCapacity);
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
    char* __Buffer = __AlAllocate(__AlString_ConversionSize);
    snprintf(__Buffer, sizeof(__AlString_ConversionSize), "%d", __N);
    return __AlStringFrom(__Buffer);
}

__AliceString* __FloatToString(double __N)
{
    char* __Buffer = __AlAllocate(__AlString_ConversionSize);
    snprintf(__Buffer, sizeof(__AlString_ConversionSize), "%f", __N);
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

__Void __AppendAlString(__AliceString* __AlString, const char* __Source)
{
    Usize __SourceLen = strlen(__Source);
    Usize __PretendedSize = (__AlString -> SSize + __SourceLen) + 1;
    if (__PretendedSize >= __AlString -> SCapacity)
    {
        Usize __NewCapacity = ((__PretendedSize * 2) + 1);
        __QMark
        char* __NewElements = __AlReallocate(__AlString -> Chars, __AlString -> SCapacity * sizeof(char), __NewCapacity * sizeof(char));
        if (__NewElements == NULL)
        {
            __QUnmark
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
    __AliceVector* __AlVector = __AlAllocate(sizeof(__AliceVector));
    __AlVector -> VElements = NULL;
    __AlVector -> ESize = __ElementSize;
    __AlVector -> VSize = 0;
    __AlVector -> VCapacity = 0;
    __AlVector -> ConversionToString = ConversionToString__AliceVector;
    return __AlVector;
}

__AliceString* ConversionToString__AliceVector(__AliceVector* __AlVector)
{
    return NULL;
}

__Void __AddCharAlString(__AliceString* __AlString, const char __Character)
{
    Usize __SourceLen = 1;
    Usize __PretendedSize = (__AlString -> SSize + __SourceLen) + 1;
    if (__PretendedSize >= __AlString -> SCapacity)
    {
        Usize __NewCapacity = ((__PretendedSize * 2) + 1);
        __QMark
        char* __NewElements = __AlReallocate(__AlString -> Chars, __AlString -> SCapacity * sizeof(char), __NewCapacity * sizeof(char));
        if (__NewElements == NULL)
        {
            __QUnmark
            return;
        }
        __AlString -> Chars = __NewElements;
        __AlString -> SCapacity = __NewCapacity;
    }
    __AlString -> Chars[__AlString -> SSize] = __Character;
    __AlString -> SSize = __AlString -> SSize + 1;
    __AlString -> Chars[__AlString -> SSize] = '\0';
}

__AliceVector* __AlVectorWithCapacity(Usize __ElementSize, Usize __Capacity)
{
    __AliceVector* __AlVector = __AlAllocate(sizeof(__AliceVector));
    __AlVector -> VElements = calloc(__Capacity, __ElementSize);
    __AlVector -> ESize = __ElementSize;
    __AlVector -> VSize = 0;
    __AlVector -> VCapacity = __Capacity;
    return __AlVector;
}

__Void __ResizeAlVector(__AliceVector* __AlVector, Usize __NewSize)
{
    if (__AlVector -> VSize > __NewSize) return;
    __QMark
    __VoidPointer __NewElements = __AlReallocate(__AlVector -> VElements, __AlVector -> VSize * __AlVector -> ESize, __NewSize * __AlVector -> ESize);
    if (__NewElements == NULL)
    {
        __QUnmark
        return;
    }
    __AlVector -> VElements = __NewElements;
    __AlVector -> VCapacity = __NewSize;
}

__Void __PushAlVector(__AliceVector* __AlVector, __VoidPointer __Element)
{
    if (__AlVector -> VSize >= __AlVector -> VCapacity)
    {
        Usize __NewCapacity = __AlVector -> VSize <= 0 ? __AlVector_StandardSize : __AlVector_SizeEquation(__AlVector -> VCapacity);
        __QMark
        __VoidPointer __NewElements = __AlReallocate(__AlVector -> VElements, __AlVector -> VSize * __AlVector -> ESize, __NewCapacity * __AlVector -> ESize);
        if (__NewElements == NULL)
        {
            __QUnmark
            return;
        }
        __AlVector -> VElements = __NewElements;
        __AlVector -> VCapacity = __NewCapacity;
    }
    char* __Destination = (char*)__AlVector -> VElements + __AlVector -> VSize * __AlVector -> ESize;
    memcpy(__Destination, __Element, __AlVector -> ESize);
    __AlVector -> VSize = __AlVector -> VSize + 1;
}

__VoidPointer __PopAlVector(__AliceVector* __AlVector)
{
    Usize __AlVectorSize = __AlVector -> VSize - 1;
    __VoidPointer __Element = __GetAlVector(__AlVector, __AlVectorSize);
    __AlVector -> VSize = __AlVector -> VSize - 1;
    return __Element;
}

__VoidPointer __GetAlVector(__AliceVector* __AlVector, Usize __Index)
{
    return (char*)__AlVector -> VElements + __Index * __AlVector -> ESize;
}

__Void __MultiplePushAlVector(__AliceVector* __AlVector, Usize __Qtd, ...)
{
    va_list __Arguments;
    va_start(__Arguments, __Qtd);
    for (int __Interator = 0; __Interator < __Qtd; __Interator++)
    {
        __PushAlVector(__AlVector, va_arg(__Arguments, void*));
    }
    va_end(__Arguments);
}


__AliceListNode* __NewAlListNode(void* __Element, __AliceListNode* __Previous, __AliceListNode* __Next)
{
    __AliceListNode* __Node = __AlAllocate(sizeof(__AliceListNode));
    __Node -> Element = __AlAllocate(sizeof(*__Element));
    memcpy(__Node -> Element, __Element, sizeof(*__Element));
    __Node -> Next = __Next;
    __Node -> Previous = __Previous;
    return __Node;
}

__AliceListNode* __AlListNodeContaining()
{

}

__AliceList* __NewAlList()
{
    __AliceList* __List = __AlAllocate(sizeof(__AliceList));
    __List -> FirstElement = NULL;
    __List -> LastElement = NULL;
    return __List;
}

__Void __AlListAppend(__AliceList* __List, void* __Element)
{
    __AliceListNode* __Node = __NewAlListNode(__Element, __List -> LastElement, NULL);
    if (__List -> FirstElement == NULL)
    {
        __List -> FirstElement = __Node;
        __List -> LastElement = __Node;
    }
    else 
    {
        __List -> LastElement -> Next = __Node;
     __List -> LastElement = __Node;
    }
}

__AliceStack* __NewAlStack(Usize __ElementSize)
{
    __AliceStack* __Stack = __AlAllocate(sizeof(__AliceStack));
    __Stack -> Elements = __NewAlVector(__ElementSize);
    __Stack -> Pointer = 0;
    return __Stack;
}

__Void __PushAlStack(__AliceStack* __Stack, __VoidPointer __Element)
{
    __PushAlVector(__Stack -> Elements, __Element);
    __Stack -> Pointer++;
}

__VoidPointer __PopAlStack(__AliceStack* __Stack)
{
    __VoidPointer __Element = __PopAlVector(__Stack -> Elements);
    __Stack -> Pointer--;
    return __Element;
}

__AliceVector* __MultiplePopAlStack(__AliceStack* __Stack, Usize __Quantity)
{
    __AliceVector* __PoppedElements = __NewAlVector(__Stack -> Elements -> ESize);
    if (__Quantity > __Stack -> Elements -> VSize)
    {
        return NULL;
    }

    for (int PopQ = 0; PopQ < __Quantity; PopQ++)
    {
        __VoidPointer __Element = __PopAlStack(__Stack);
        __PushAlVector(__PoppedElements, __Element);
    }
    return __PoppedElements;
}