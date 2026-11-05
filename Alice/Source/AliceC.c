#include "Compiler/AliceC.h"
#include "Compiler/AliceCompilerUtilities.h"
#include "Headers/AliceTypes.h"

CodeBlock* CLang_Include(const char* __LibName)
{
    CodeBlock* B = CBlock();
    CodeLine* L = CLine("#include \"");
    AppendStr(L -> Content, __LibName);
    AppendStr(L -> Content, "\"");
    CBlockAppend(B, L);
    return B;
}

CodeLine* CLang_InitVariable(const char* __Name, const __AliceType* __Type, CodeBlock* __Block)
{
    String* TypeL = AliceTypeToCType(__Type);
    CodeLine* L = CLine(TypeL -> Chars);

    AppendStr(L -> Content, " ");
    AppendStr(L -> Content, __Name);
    AppendStr(L -> Content, ";");

    if (__Block != NULL) CBlockAppend(__Block, L);

    return L;
}

CodeBlock* CLang_Function(const char* __Name, Vector* __Arguments, const __AliceType* __ReturnType)
{
    FromStr(__Name);
}

CodeBlock* CLang_Main()
{
    CodeBlock* MB = CBlock();
    QFORMAT(Opening, "int main(int __ARGUMENTS_COUNT__, char* __ARGUMENTS_VALUES__[])\n{")
    QFORMAT(Closure, "}")
    CBlockFormat(MB, Opening, Closure);
    return MB;
}

String* AliceTypeToCType(const __AliceType* __Type)
{
    switch (__Type -> Kind)
    {
        case AlType_i8: return FromStr("int8_t");
        case AlType_i16: return FromStr("int16_t");
        case AlType_i32: return FromStr("int32_t");
        case AlType_i64: return FromStr("int64_t");
        case AlType_u8: return FromStr("uint8_t");
        case AlType_u16: return FromStr("uint16_t");
        case AlType_u32: return FromStr("uint32_t");
        case AlType_u64: return FromStr("uint64_t");
        case AlType_f32: return FromStr("float");
        case AlType_f64: return FromStr("double");
        case AlType_Char: return FromStr("char");
        case AlType_List: return FromStr("__AliceList*");
        case AlType_String: return FromStr("__AliceString*");
        case AlType_Vector: return FromStr("__AliceVector*");
        default: return FromStr("");
    }
}