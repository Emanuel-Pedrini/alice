#include "External/Libraries.h"
#include "Headers/ArchiveManager.h"
#include "Headers/Lexer.h"
#include "Headers/Parser.h"
#include "Headers/Reader.h"
#include "Compiler/AliceCompilerUtilities.h"
#include "Headers/AliceTypes.h"
#include "Headers/Parser.h"
#include "Headers/AIR.h"
#include "Utilities/AliceDevArraySortsTools.h"
#include "Headers/PrattParsing.h"
#include "Headers/SemanticAnalysis.h"
#include "Compiler/Native/AliceLLVM.h"

Bool ReadAndDecode(const int argc, CString* argv)
{
    AliceCompilationAttributesReader* CompilationReader = NewAliceCompilationReader();
    AliceCompilationAttributes* CompilationAttributes = ReadCompilationAttributes(CompilationReader, argc, argv);
    AliceManagementObject* BigGirl = ManageCompilationObject(CompilationReader);

    __AliceVector* Tokens = AliceLexerTokenize(BigGirl -> SourceCode);
    {

    /* 
    **TEMPORARY : ONLY FOR TESTS, REMOVE LATER. 
    */
    __AliceString* x = __AlStringFrom("");
    for (int i = 0 ; i < Tokens -> VSize ; i++) 
    {
        char p[50];
        AliceToken* t = (AliceToken*)__GetAlVector(Tokens, i);
        __AliceString* s = __AlStringFrom("");
        char* c = t -> Literal;
        if (t -> Type == Tk_NewLine) 
        {
            s = __AlStringFrom("\\n");
        }
        else 
        {
            s = __AlStringFrom(c);
        }
        snprintf(p, sizeof(p), "(%d) ", t -> Type);
        __AppendAlString(x, p);
        __AppendAlString(x, "\"");
        __AppendAlString(x, s -> Chars);
        __AppendAlString(x, "\"");
        __AppendAlString(x, "\n");
    }
        __AliceString* s = __AlStringFrom(CompilationAttributes -> TargetArchive -> Chars);   
        __AppendAlString(s, ".txt");
        FileWrite(s -> Chars, x -> Chars, "w");
    }

    ParserState* AliceParserState = NewParserState(Tokens);
    ParserNode* AliceProgramAST = ParsenizeParserState(AliceParserState);
    SemanticAnalyserState* AliceSemanticAnalyser = NewSemanticAnalyserState(AliceProgramAST);
    SemanticAnalysis* AliceSemanticAnalysisResult = AnalyseSemanticAnalyserState(AliceSemanticAnalyser);
    AliceIRBuilder* AliceBuilder = NewAlIRBuilder();
    AliceIRModule* AliceIR = BuildIRFromTypedASTIRBuilder(AliceBuilder, AliceProgramAST);
    
    // Temporary structure
    __AliceString* CompilationPath = __AlStringFrom(CompilationAttributes -> TargetArchive -> Chars);
    __AppendAlString(CompilationPath, ".ll");
    AliceCodeWritter* AliceSourceToTextCompiler = NewAliceCodeWritter(CompilationPath -> Chars);
    AliceCompilerResult* CompilationResult = CompileToLLVMFromAIR(AliceSourceToTextCompiler, AliceIR);
    CodeWrite(AliceSourceToTextCompiler);
    return True;
}

int VirtualMain(const int argc, CString* argv)
{
    ReadAndDecode(argc, argv);
    __AlFreeAll();
    return 0;
}

int DebugMain(long Flag)
{
    switch (Flag)
    {
    case 0:
        __AliceString* S = __AlStringFrom("");
        char ActualInput = '\0';
        while (ActualInput != '\n')
        {
            ActualInput = getchar();
            __AddCharAlString(S, ActualInput);
        }
        __AliceVector* Tokens = AliceLexerTokenize(S);
        __AliceVector* CleanTokens = __NewAlVector(sizeof(AliceToken));
        __ForIn(Tokens, I, 0) 
        {
            AliceToken* T = __GetAlVector(Tokens, I);
            if (!(T -> Type == Tk_EndOfCode || T -> Type == Tk_ArchiveMarker))
            {
                __PushAlVector(CleanTokens, T);
            }
        }
        ExpressionReader* R = NewExpressionReader();
        R -> ConstExpression = CleanTokens;
        R -> Pointer = 0;
        Expression* E = ParseExpression(R, 0);
        return 0;
    case 1:
        __AliceType* T0 = __NewAlGenericType("i32", AlType_i32, NULL);
        __AliceType* T1 = __NewAlGenericType("Array", AlType_Generic, T0);
        __AliceType* T2 = __NewAlGenericType("Array", AlType_Generic, T1);
        __AliceString* S1 = T2 -> ConversionToString(T2);
        printf("%s", S1 -> Chars);
    case 2:
        Dsize _1 = 145;
        Dsize _2 = 2;
        __AliceDictionary* D = __NewAlDictionaryWithElements(sizeof(Dsize), 4, 
        __AlStringFrom("UM"), &_1, 
        __AlStringFrom("DOIS"), &_2);
        printf("\n>> %d", *((Dsize*)__GetAlDictionary(D, __AlStringFrom("UM"))));
    case 3:
        AliceCodeWritter* C = NewAliceCodeWritter("C:\\Users\\Emanuel\\Documents\\GitHub\\alice\\comptest.txt");
        AliceCodeBlock* B = NewCodeBlockFormatted("@define i32 main() {", "\n}");
        AliceCodeLine* L1 = NewCodeLine("\nentry:");
        AliceCodeLine* L2 = NewCodeLine("\n   ret i32 0");
        NewCodeBlockLine(B, L1);
        NewCodeBlockLine(B, L2);
        NewCodeWriterBlock(C, B);
        CodeWrite(C);
    default:
        return 0;
    }
}

int main(const int argc, CString argv[]) 
{
    if (strcasecmp(argv[argc - 2], "alicedbg") == 0) return DebugMain(__StringToInt(__AlStringFrom(argv[argc - 1]), 10));
    return VirtualMain(argc, argv);
}


