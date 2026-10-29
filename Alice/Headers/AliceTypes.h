#ifndef _AliceTypesH_
#define _AliceTypesH_

#include "Headers/StringUtilities.h"
#include "External/Libraries.h"

typedef void __AliceVoid;
typedef void* __AliceVoidPtr;

typedef enum EnumAliceTypeKind
{
    i8 = 0,
    i16,
    i32,
    i64,
    i128,
    u8,
    u16,
    u32,
    u64,
    u128,
    f32,
    f64,
    AlChar,
    AlString,
    AlVector,
    AlCustomType
} TypeKind;
#define TPK (f64 - i8)

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

typedef struct StructAliceType
{
    String* TypeLiteral;
    TypeKind Kind;
} AliceType;

#endif