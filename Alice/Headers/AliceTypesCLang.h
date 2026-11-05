#ifndef _AliceTypesCLangH_
#define _AliceTypesCLangH_

#include "Headers/StringUtilities.h"
#include "External/Libraries.h"
#include "Headers/AliceTypes.h"

typedef struct Struct__AliceListNode __AliceListNode;

typedef enum EnumAliceTypeKind
{
    AlType_Null,
    AlType_i8 = 0,
    AlType_i16,
    AlType_i32,
    AlType_i64,
    AlType_i128,
    AlType_u8,
    AlType_u16,
    AlType_u32,
    AlType_u64,
    AlType_u128,
    AlType_f32,
    AlType_f64,
    AlType_Char,
    AlType_String,
    AlType_List,
    AlType_Vector,
    AlType_Userdata
} TypeKind;
#define TPK (AlType_f64 - AlType_i8)

typedef struct Struct__AliceRefCounted
{

} __AliceRefCounted;

typedef struct Struct__AliceVector
{
    void* VElements;
    Usize VSize;
    Usize VCapacity;
    Usize ESize;
} __AliceVector;

__AliceVector* __NewAlVector(Usize __ElementSize);
__AliceVector* __AlVectorWithCapacity(Usize __ElementSize, Usize __Capacity);
__AliceVoid __ResizeAlVector(__AliceVector* __AlVector, Usize __NewSize);
__AliceVoid __PushAlVector(__AliceVector* __AlVector, __AliceVoidPtr __Element);
__AliceVoid __MultiplePushAlVector(__AliceVector* __AlVector, Usize __Qtd, ...);
__AliceVoid __PopAlVector(__AliceVector* __AlVector);
__AliceVoidPtr __GetAlVector(__AliceVector* __AlVector, Usize __Index);
#define __ForIn(V, I, S) for (int I = S ; I < V -> VSize; I++)
#define __AlVector(V, E) __AliceVector* V = __NewAlVector(sizeof(E));

typedef struct Struct__AliceString
{
    char* Chars;
    Usize SSize;
    Usize SCapacity;
} __AliceString;

__AliceString* __NewAlString();
__AliceString* __AlStringFrom(const char* __Source);
__AliceString* __ConcatenateAlString(Usize __Qtd, ...);
__AliceVoid __AppendAlString(__AliceString* __AlString, const char* __Source);
__AliceString* __JoinAlString(const char* __Separator, Usize __Qtd, ...);
__AliceString* __FloatToString(double __N);
__AliceString* __IntToString(long int __N);
double __StringToDouble(__AliceString* __AlString);
long int __StringToInt(__AliceString* __AlString, Usize __Base);
__AliceVoid __DropAlString(void* __AlString);

typedef struct Struct__AliceListNode
{
    const __AliceType* Type;
    __AliceVoidPtr Element;
    __AliceListNode* Previous;
    __AliceListNode* Next;
} __AliceListNode;

typedef struct Struct__AliceList
{
    __AliceListNode* FirstElement;
    __AliceListNode* LastElement;
} __AliceList;

__AliceListNode* __NewAlListNode(void* __Element, const __AliceType* __Type, __AliceListNode* __Previous, __AliceListNode* __Next);
__AliceListNode* __AlListNodeContaining();
__AliceList* __NewAlList();
__AliceVoid __AlListAppend(__AliceList* __List, void* __Element, const __AliceType* __Type);

#endif