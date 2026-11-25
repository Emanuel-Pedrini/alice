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
#include "Compiler/Native/AliceLLVMAPI.h"

Bool ReadAndDecode(const int argc, CString* argv)
{
    AliceCompilationAttributesReaderRef CompilationReader = NewAliceCompilationReader();
    AliceCompilationAttributesRef CompilationAttributes = ReadCompilationAttributes(CompilationReader, argc, argv);
    AliceManagementObjectRef BigGirl = ManageCompilationObject(CompilationReader);

    __AliceString* CompilationPath = __AlStringFrom(CompilationAttributes -> TargetArchive -> Chars);
    AliceCodeWritter* AliceSourceToTextCompiler = NewAliceCodeWritter(CompilationPath -> Chars);

    __AliceVector* Tokens = AliceLexerTokenize(BigGirl);

    ParserState* AliceParserState = NewParserState(Tokens);
    ParserNode* AliceProgramAST = ParsenizeParserState(AliceParserState);
    
    SemanticAnalyserStateRef AliceSemanticAnalyser = NewSemanticAnalyserState(AliceProgramAST);

    SemanticAnalysisResultRef AliceSemanticAnalysisResult = AnalyseSemanticAnalyserState(AliceSemanticAnalyser);
    if (!GetSemanticAnalysisWasSucessfull(AliceSemanticAnalysisResult)) goto AliceCompilationDone;

    AliceIRBuilder* AliceBuilder = NewAlIRBuilder();

    AliceIRModule* AliceIR = BuildIRFromTypedASTIRBuilder(AliceBuilder, AliceProgramAST);

    if (__AlDictionaryContains(CompilationReader -> Attributes -> FlagValues, __AlStringFrom("ll")))
    {
        AliceCompilerResult* CompilationResult = CompileToLLVMFromAIR(AliceSourceToTextCompiler, AliceIR);
        if (!GetCompilationWasSucessfull(CompilationResult)) goto AliceCompilationDone;

        __AliceString* OutputPath = CompilationReader -> Attributes -> TargetArchive;
        GenerateExecutableFromObjectFile(CompilationResult -> RLiteral -> Chars, OutputPath -> Chars, "default<O2>", False, False);
    }
    return True;

    AliceCompilationDone:
        return False;
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
            break;
        default:
            return 0;
    }
}

int main(const int argc, CString* argv) 
{
    if (strcasecmp(argv[argc - 2], "alicedbg") == 0) return DebugMain(__StringToInt(__AlStringFrom(argv[argc - 1]), 10));
    return VirtualMain(argc, argv);
}


