#ifndef _AliceTypesCLangH_
#define _AliceTypesCLangH_

#include "External/Libraries.h"
#include "Headers/AliceMemoryManagement.h"
#include "Headers/Lexer.h"
#include "Headers/ForwardInclude.h"
#include "Headers/ForwardInclude.h"

#define True ((Bool)1)
#define False ((Bool)0)
#define __ForIn(V, I, S) for (int I = S ; I < V -> VSize; I++)
#define __Max(A, B) ((A) > (B)) ? (A) : (B);
#define __AlVector(V, E) __AliceVector* V = __NewAlVector(sizeof(E));

typedef struct Struct__AliceListNode __AliceListNode;
typedef struct Struct__AliceListNode __AliceListNode;
typedef struct Struct__AliceList __AliceList;
typedef struct Struct__AliceRefCounted __AliceRefCounted;
typedef struct Struct__AliceDictionary __AliceDictionary;

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
__Void __InsertAlVector(__AliceVector* __AlVector, __VoidPointer __Element, Usize __Index);
__Void __HardInsertAlVector(__AliceVector* __AlVector, __VoidPointer __Element, Usize __Index);

struct Struct__AliceString
{
    char* Chars;
    Usize SSize;
    Usize SCapacity;
    __AliceRefCounted* RefCount;
};

__AliceString* __NewAlString();
__AliceString* __AlStringFrom(CString __Source);
__AliceString* __ConcatenateAlString(Usize __Qtd, ...);
__Void __AppendAlString(__AliceString* __AlString, CString __Source);
Bool __AlStringCompare(__AliceString* __AlString, __AliceString* __AlStringSource);
__AliceString* __JoinAlString(CString __Separator, Usize __Qtd, ...);
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
__VoidPointer __LookAlStack(__AliceStack* __Stack);
__VoidPointer __DeepLookAlStack(__AliceStack* __Stack, Usize __Depth);

struct Struct__AliceDictionary
{
    __AliceVector* Keys; // Vector<Usize>*
    __AliceVector* Values; // Vector<Generic>*
    Usize Size;
};

__AliceDictionary* __NewAlDictionary(Usize __ValueSize);
Usize __HashCodeFromString(__AliceString* __Key);
Usize __IndexFromStringAlDictionary(__AliceDictionary* __AlDictionary, __AliceString* __Key);
Bool __NewElementAlDictionary(__AliceDictionary* __AlDictionary, __AliceString* __Key, __VoidPointer __Value);
__VoidPointer __GetAlDictionary(__AliceDictionary* __AlDictionary, __AliceString* __Key);
__Void __MultipleNewElementsAlDictionary(__AliceDictionary* __AlDictionary, Usize __Qtd, ...);
__AliceDictionary* __NewAlDictionaryWithElements(Usize __ValueSize, Usize __Qtd, ...);
Bool __AlDictionaryContains(__AliceDictionary* __AlDictionary, __AliceString* __Key);
#endif