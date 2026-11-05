#ifndef _AliceTypesH_
#define _AliceTypesH_

#include "Headers/InterpreterTypes.h"
#include "External/Libraries.h"

typedef void __AliceVoid;
typedef void* __AliceVoidPtr;

typedef struct Struct__AliceType __AliceType;

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

struct Struct__AliceType
{
    const char* Literal;
    TypeKind Kind;
    Usize Size;
    const __AliceType* Inner;
    __AliceVoid (*__Drop) (__AliceVoidPtr __Value);
};

extern const __AliceType __AliceTypePtr_Null;
extern const __AliceType __AliceTypePtr_i8;
extern const __AliceType __AliceTypePtr_i16;
extern const __AliceType __AliceTypePtr_i32;
extern const __AliceType __AliceTypePtr_i64;
extern const __AliceType __AliceTypePtr_u8;
extern const __AliceType __AliceTypePtr_u16;
extern const __AliceType __AliceTypePtr_u32;
extern const __AliceType __AliceTypePtr_u64;
extern const __AliceType __AliceTypePtr_f32;
extern const __AliceType __AliceTypePtr_f64;
extern const __AliceType __AliceTypePtr_Char; 
extern const __AliceType __AliceTypePtr_String;
#endif