#include "Compiler/AliceC.h"
#include "Compiler/AliceCompilerUtilities.h"
#include "Headers/AliceTypes.h"

CodeBlock* CLang_Include(const char* LibName)
{
    CodeBlock* B = CBlock();
    CodeLine* L = CLine("#include \"");
    AppendStr(L -> Content, LibName);
    AppendStr(L -> Content, "\"");
    CBlockAppend(B, L);
    return B;
}

CodeLine* CLang_InitVariable(const char* Name, AliceType* Type)
{
    String* TypeL = AliceTypeToCType(Type);
    CodeLine* L = CLine(TypeL -> Chars);
    AppendStr(L -> Content, " ");
    AppendStr(L -> Content, Name);
    AppendStr(L -> Content, ";");
    return L;
}

CodeLine* CLang_NonInitVariable(const char* Name, AliceType* Type)
{

}

CodeBlock* CLang_Function(const char* Name, Vector* Args, CodeBlock* Body, AliceType* RType)
{
}

CodeBlock* CLang_Main(CodeBlock* Body)
{
    CodeBlock* MB = Body;
    QFORMAT(Opening, "int main(int __ARGUMENTS_COUNT__, char* __ARGUMENTS_VALUES__[])\n{")
    QFORMAT(Closure, "}")
    CBlockFormat(MB, Opening, Closure);
    return MB;
}

String* AliceTypeToCType(AliceType* T)
{
    char* MapAliceTypeToCType[] = {
        "short int", 
        "int", 
        "long int", 
        "long long int", 
        "long long int", 
        "unsigned short int",
        "unsigned int", 
        "unsigned long int",
        "unsigned long long int",
        "unsigned long long int",
        "float",
        "double"
    };
    TypeKind TS = T -> Kind;
    if (TS < (TPK + 1))
    {
        return FromStr(MapAliceTypeToCType[TS]);
    }
    return FromStr("");
}