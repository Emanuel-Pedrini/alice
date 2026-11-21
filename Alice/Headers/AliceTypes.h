#ifndef _AliceTypesH_
#define _AliceTypesH_

#include "Headers/AliceMemoryManagement.h"
#include "Headers/AliceDevelopmentTypes.h"
#include "Headers/PrattParsing.h"
#include "External/Libraries.h"
#include "Headers/Lexer.h"
#include "Headers/ForwardInclude.h"

extern __AliceDictionary* __GlobaStringToAlTypelDictionary;

typedef enum
{
    AlType_Null = 0,
    AlType_i8,
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
    AlType_Array,
    AlType_Boolean,
    AlType_Pointer,
    AlType_Generic,
    AlType_Userdata
} AliceTypeKind;
#define TPK (AlType_f64 - AlType_i8)

struct Struct__AliceType
{
    const char* Literal;
    AliceTypeKind Kind;
    __AliceType* RecursiveType;
    Expression* SizeExpression;
    __AliceVector* InternalFlags;
    Bool Flagged;
    Bool Sized;
    __AliceString* (*ConversionToString)(__AliceType*);
};

__AliceType* __NewAlGenericType(const char* __Literal, AliceTypeKind __Kind, __AliceType* __RecursiveType);
__AliceString* ConversionToString__AliceType(__AliceType* __AlType);
AliceTypeKind __NativeAlTypeFromString(__AliceString* __Key, Bool __Override);
Bool __CompareAliceType(__AliceType* Type, __AliceType* OtherType);
Bool __IsBooleanOperator(AliceTokenType Type);
#endif