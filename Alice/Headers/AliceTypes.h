#ifndef _AliceTypesH_
#define _AliceTypesH_

#include "Headers/StringUtilities.h"
#include "External/Libraries.h"

typedef void __AliceVoid;

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
    void* EPointer;
    Usize VSize;
    Usize VCapacity;
    Usize ESize;
} __AliceVector;

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

typedef struct StructAliceType
{
    String* TypeLiteral;
    TypeKind Kind;
} AliceType;

#endif