#include "External/Libraries.h"
#include "Headers/ArchiveManager.h"
#include "Headers/StringUtilities.h"
#include "Headers/InterpreterConstants.h"
#include "Headers/Lexer.h"
#include "Headers/Parser.h"
#include "Headers/Cleaner.h"
#include "Headers/Reader.h"

int main(int argc, char* argv[]) {
    CompAttributes* CompilationAttributes = Reader(argc, argv);
    Alice_BigGirl BigGirl;
    Vector Tokens;
    New(&Tokens, sizeof(Alice_Token));
    GirlInit(&BigGirl);
    ReadPath(CompilationAttributes, &BigGirl);
    Clean(&BigGirl);
    Tokenize(&BigGirl, &Tokens);
    Parsenize(&BigGirl, &Tokens);
    return 0;
}
