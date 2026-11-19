#ifndef _AliceTypesCLangH_
#define _AliceTypesCLangH_

#include "External/Libraries.h"

#define True ((Bool)1)
#define False ((Bool)0)
#define __ForIn(V, I, S) for (int I = S ; I < V -> VSize; I++)
#define __AlVector(V, E) __AliceVector* V = __NewAlVector(sizeof(E));

typedef struct Struct__AliceListNode __AliceListNode;
typedef unsigned short int Bool;
typedef unsigned long long int Usize;
typedef struct Struct__AliceListNode __AliceListNode;
typedef struct Struct__AliceList __AliceList;
typedef struct Struct__AliceString __AliceString;
typedef struct Struct__AliceVector __AliceVector;
typedef struct Struct__AliceRefCounted __AliceRefCounted;
typedef struct Struct__AliceStack __AliceStack;

typedef void __Void;
typedef void* __VoidPointer;

struct Struct__AliceRefCounted
{
    Bool AlreadyFree;
};

struct Struct__AliceVector
{
    void* VElements;
    Usize VSize;
    Usize VCapacity;
    Usize ESize;
    __AliceString* (*ConversionToString)(__AliceVector*);
    __AliceRefCounted* RefCount;
};

__AliceVector* __NewAlVector(Usize __ElementSize);
__AliceVector* __AlVectorWithCapacity(Usize __ElementSize, Usize __Capacity);
__Void __ResizeAlVector(__AliceVector* __AlVector, Usize __NewSize);
__Void __PushAlVector(__AliceVector* __AlVector, __VoidPointer __Element);
__Void __MultiplePushAlVector(__AliceVector* __AlVector, Usize __Qtd, ...);
__VoidPointer __PopAlVector(__AliceVector* __AlVector);
__VoidPointer __GetAlVector(__AliceVector* __AlVector, Usize __Index);
__AliceString* ConversionToString__AliceVector(__AliceVector* __AlVector);

struct Struct__AliceString
{
    char* Chars;
    Usize SSize;
    Usize SCapacity;
    __AliceRefCounted* RefCount;
};

__AliceString* __NewAlString();
__AliceString* __AlStringFrom(const char* __Source);
__AliceString* __ConcatenateAlString(Usize __Qtd, ...);
__Void __AppendAlString(__AliceString* __AlString, const char* __Source);
__AliceString* __JoinAlString(const char* __Separator, Usize __Qtd, ...);
__AliceString* __FloatToString(double __N);
__AliceString* __IntToString(long int __N);
double __StringToDouble(__AliceString* __AlString);
long int __StringToInt(__AliceString* __AlString, Usize __Base);
__Void __DropAlString(void* __AlString);
__Void __AddCharAlString(__AliceString* __AlString, const char __Character);

struct Struct__AliceListNode
{
    __VoidPointer Element;
    __AliceListNode* Previous;
    __AliceListNode* Next;
    __AliceRefCounted* RefCount;
};

struct Struct__AliceList
{
    __AliceListNode* FirstElement;
    __AliceListNode* LastElement;
    __AliceRefCounted* RefCount;
};

__AliceListNode* __NewAlListNode(void* __Element, __AliceListNode* __Previous, __AliceListNode* __Next);
__AliceListNode* __AlListNodeContaining();
__AliceList* __NewAlList();
__Void __AlListAppend(__AliceList* __List, void* __Element);

struct Struct__AliceStack
{
    Usize Pointer;
    __AliceVector* Elements;
    __AliceRefCounted* RefCount;
};

__AliceStack* __NewAlStack(Usize __ElementSize);
__Void __PushAlStack(__AliceStack* __Stack, __VoidPointer __Element);
__VoidPointer __PopAlStack(__AliceStack* __Stack);
__AliceVector* __MultiplePopAlStack(__AliceStack* __Stack, Usize __Quantity);
#endif