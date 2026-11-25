#include "Headers/AliceTypes.h"
#include "External/Libraries.h"

#define __DeclareConstantType(Type, NType, Signed, BitWidth) __AliceType Global##Type = {\
.Literal = #Type,\
.Kind = AlType_##Type,\
.Information.Numeric = {\
NType,\
Signed,\
BitWidth\
}};

__DeclareConstantType(i8, AlNumericType_Integer, True, 8)
__DeclareConstantType(i16, AlNumericType_Integer, True, 16)
__DeclareConstantType(i32, AlNumericType_Integer, True, 32)
__DeclareConstantType(i64, AlNumericType_Integer, True, 64)
__DeclareConstantType(u8, AlNumericType_Integer, False, 8)
__DeclareConstantType(u16, AlNumericType_Integer, False, 16)
__DeclareConstantType(u32, AlNumericType_Integer, False, 32)
__DeclareConstantType(u64, AlNumericType_Integer, False, 64)
__DeclareConstantType(i128, AlNumericType_Integer, True, 128)
__DeclareConstantType(u128, AlNumericType_Integer, False, 128)
__DeclareConstantType(f32, AlNumericType_Floating, True, 32)
__DeclareConstantType(f64, AlNumericType_Floating, True, 64)
__DeclareConstantType(Boolean, AlNumericType_Integer, False, 1)
__DeclareConstantType(Char, AlNumericType_Integer, False, 8)
__DeclareConstantType(Null, AlNumericType_Integer, False, 8)
__DeclareConstantType(Pointer, AlNumericType_Integer, False, 8)

Bool __IsBooleanOperator(AliceTokenType Type)
{
    switch (Type)
    {
        case Tk_LeftArrow:
        case Tk_RightArrow:
        case Tk_GreaterEqual:
        case Tk_LessEqual:
        case Tk_EqualEqual:
        case Tk_NotEqual:
        case Tk_And:
        case Tk_Or:
        case Tk_Not:
            return True;
        default:
            return False;
    }
}

__AliceType* __NewAliceType(CString __Literal, AliceTypeKind __Kind, __AliceType* __InternType)
{
    __AliceType* __AlType = __AlAllocate(sizeof(__AliceType));
    __AlType -> Kind = __Kind;
    __AlType -> Literal = __Literal;
    __AlType -> ConversionToString = ConversionToString__AliceType;
    if (__Kind = AlType_Generic)
    {
        __AlType -> Information.Generic.ElementType = __InternType;
        __AlType -> Information.Generic.SizeExpressed = False;
        __AlType -> Information.Generic.SizeExpression = NULL;
    }
    return __AlType;
}

__AliceString* ConversionToString__AliceType(__AliceType* __AlType)
{
    if (__AlType == NULL) return __AlStringFrom("Null");
    else if (__AlType -> Kind == AlType_Pointer) 
    {
        __AliceString* __Result = __AlStringFrom("Ptr<( ");
        __AppendAlString(__Result, ConversionToString__AliceType(__AlType -> Information.Pointer.ElementType) -> Chars);
        __AppendAlString(__Result, " )>");
        return __Result;
    }
    else {
        return __AlStringFrom(__AlType -> Literal);
    }
}

Bool __CompareAliceType(__AliceType* Type, __AliceType* OtherType)
{
    if (Type == OtherType) return True;
    else if (Type -> Kind != OtherType -> Kind) return False;
    else if (Type -> Kind == AlType_Userdata) return (strcmp(Type -> Literal, OtherType -> Literal) == 0);
    else if (Type -> Kind == AlType_Generic) return __CompareAliceType(
        Type -> Information.Generic.ElementType, 
        OtherType -> Information.Generic.ElementType
    );
    return True;
}

__AliceDictionary* __GlobaStringToAlTypelDictionary = NULL;
#define __StringKind(K) AliceTypeKind PointerTo##K = AlType_##K;__NewElementAlDictionary(__GlobaStringToAlTypelDictionary, __AlStringFrom(#K), &Global##K);
__AliceType*__NativeAlTypeFromString(__AliceString* __Key, Bool __Override)
{
    if (__GlobaStringToAlTypelDictionary == NULL) 
    {
        __GlobaStringToAlTypelDictionary = __NewAlDictionary(sizeof(__AliceType*));
        __StringKind(u8)
        __StringKind(u16)
        __StringKind(u32)
        __StringKind(u64)
        __StringKind(i8)
        __StringKind(i16)
        __StringKind(i32)
        __StringKind(i64)
        __StringKind(f32)
        __StringKind(f64)
        __StringKind(Null)
    }
    if (__AlDictionaryContains(__GlobaStringToAlTypelDictionary, __Key) && !__Override)
        return (*(__AliceType**)__GetAlDictionary(__GlobaStringToAlTypelDictionary, __Key));
    return NULL;
}

AliceCastingType __CastingType(__AliceType* FromType, __AliceType* ToType)
{
    if (FromType == ToType) return AlCastingType_NoCasting;
    if (__NumericType(FromType -> Kind) && __NumericType(ToType -> Kind))
    {
        __AliceTypeNumericInformation FromNumericInfo = FromType -> Information.Numeric;
        __AliceTypeNumericInformation ToNumericInfo = ToType -> Information.Numeric;

        Bool SameSigning = FromNumericInfo.IsSigned == ToNumericInfo.IsSigned;
        Bool FloatToInteger = FromNumericInfo.Type > ToNumericInfo.Type;
        Bool ToABiggerSize = ToNumericInfo.BitWidth >= FromNumericInfo.BitWidth;
        if (!SameSigning) return AlCastingType_Explicit;
        else if (!ToABiggerSize) return AlCastingType_Explicit;
        else if (FloatToInteger) return AlCastingType_Explicit;
        else return AlCastingType_Implicit;
    }
    else
    {
        return AlCastingType_Invalid;
    }
}