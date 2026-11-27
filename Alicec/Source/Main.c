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

int AliceReadAndDecodeMain(CString* Arguments, const int NumberOfArguments)
{
    AliceCompilationAttributesReaderRef CompilationReader = NewAliceCompilationReader();
    AliceCompilationAttributesRef CompilationAttributes = ReadCompilationAttributes(CompilationReader, NumberOfArguments, Arguments);
    AliceManagementObjectRef BigGirl = ManageCompilationObject(CompilationReader);

    __AliceString* TargetCArchive = CompilationAttributes -> TargetArchive;
    if (TargetCArchive == NULL) goto AliceCompilationDone;

    __AliceString* CompilationPath = __AlStringFrom(TargetCArchive -> Chars);
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
    return 0;

    AliceCompilationDone:
        return 1;
}

int VirtualMain(const int argc, CString* argv)
{
    int ReadDecodeResult = AliceReadAndDecodeMain(argv, argc);
    __AlFreeAll();
    return ReadDecodeResult;
}

int main(const int argc, CString* argv) 
{
    return VirtualMain(argc, argv);
}


