#include "Headers/AliceTypes.h"
#include "External/Libraries.h"

typedef enum 
{
    RTTAlType,
    RTTExpression,
    RTTNonRecursive
} RecursiveTypeKind;

typedef union Union__RecursiveType
{
    __AliceType* Type;
    Expression* Expression;
} RecursiveType;

typedef struct Struct__AlRTFlag
{
    RecursiveTypeKind Kind;
    RecursiveType Type;
} AlRTFlag;

Bool __IsBooleanOperator(AliceTokenType Type)
{
    switch (Type)
    {
        case Tk_LeftArrow:
        case Tk_RightArrow:
        case Tk_And:
        case Tk_Or:
        case Tk_Not:
            return True;
        default:
            return False;
    }
}

__AliceType* __NewAlGenericType(const char* __Literal, AliceTypeKind __Kind, __AliceType* __RecursiveType)
{
    __AliceType* __AlType = __AlAllocate(sizeof(__AliceType));
    __AlType -> Kind = __Kind;
    __AlType -> Literal = __Literal;
    __AlType -> ConversionToString = ConversionToString__AliceType;
    __AlType -> RecursiveType = __RecursiveType;
    __AlType -> InternalFlags = __NewAlVector(sizeof(AlRTFlag));
    __AlType -> Flagged = False;
    __AlType -> Sized = False;
    __AlType -> SizeExpression = NULL;
    return __AlType;
}

__Void __SetAlGenericTypeFlag(__AliceType* __AlType, __AliceVector* __Flags)
{
    __AlType -> InternalFlags = __Flags;
    __AlType -> Flagged = True;
}

__AliceString* ConversionToString__AliceType(__AliceType* __AlType)
{
    if (__AlType == NULL) return __AlStringFrom("Null");
    else if (__AlType -> Kind == AlType_Pointer) 
    {
        __AliceString* __Result = __AlStringFrom("Ptr<( ");
        __AppendAlString(__Result, ConversionToString__AliceType(__AlType -> RecursiveType) -> Chars);
        __AppendAlString(__Result, " )>");
        return __Result;
    }
    else if (__AlType -> Kind == AlType_Generic)
    {
        __AliceString* __Result = __AlStringFrom("Generic<( ");
        __AppendAlString(__Result, ConversionToString__AliceType(__AlType -> RecursiveType) -> Chars);
        if (__AlType -> Sized) 
        {
            __AppendAlString(__Result, " , ");
            __AppendAlString(__Result, __AlType -> SizeExpression -> ConversionToString(__AlType -> SizeExpression) -> Chars);
        }
        __AppendAlString(__Result, " )>");
        return __Result;
    }
    else {
        return __AlStringFrom(__AlType -> Literal);
    }
}

Bool __CompareAliceType(__AliceType* Type, __AliceType* OtherType)
{
    AliceTypeKind T = Type -> Kind;
    if (Type -> Kind != OtherType -> Kind) return False;
    if (T == AlType_Userdata) return (strcmp(Type -> Literal, OtherType -> Literal) == 0);
    if (T == AlType_Generic) return __CompareAliceType(Type -> RecursiveType, OtherType -> RecursiveType);
    return True;
}

__AliceDictionary* __GlobaStringToAlTypelDictionary = NULL;
#define Kind(K) AliceTypeKind PointerTo##K = AlType_##K;__NewElementAlDictionary(__GlobaStringToAlTypelDictionary, __AlStringFrom(#K), &PointerTo##K);
AliceTypeKind __NativeAlTypeFromString(__AliceString* __Key, Bool __Override)
{
    if (__GlobaStringToAlTypelDictionary == NULL) 
    {
        __GlobaStringToAlTypelDictionary = __NewAlDictionary(sizeof(AliceTypeKind));
        Kind(u8)
        Kind(u16)
        Kind(u32)
        Kind(u64)
        Kind(i8)
        Kind(i16)
        Kind(i32)
        Kind(i64)
        Kind(f32)
        Kind(f64)
        Kind(Null)
        Kind(Pointer)
    }
    if (__AlDictionaryContains(__GlobaStringToAlTypelDictionary, __Key) && !__Override)
        return *((AliceTypeKind*)__GetAlDictionary(__GlobaStringToAlTypelDictionary, __Key));
    return AlType_Userdata;
}