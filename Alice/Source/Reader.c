#include "External/Libraries.h"
#include "Headers/ArchiveManager.h"
#include "Headers/StringUtilities.h"
#include "Headers/InterpreterConstants.h"
#include "Headers/Lexer.h"
#include "Headers/Cleaner.h"
#include "Headers/Reader.h"
#include "Headers/Lexer.h"

CompAttributes* Reader(int NumberOfArguments, char** Arguments) 
{
    CompAttributes* Attributes = malloc(sizeof(CompAttributes));
    Attributes -> Paths = FromStr("");

    Usize Count = 0;
    Usize x = 0;

    printf("Including...\n");
    
    while (Count < NumberOfArguments) 
    {
        char* Argument = Arguments[Count];
        if (EndsWith(Argument, ALICE_SUFFIX)) 
        {
            String* s = FromStr(Argument);
            printf(">> %s \n", s -> Chars);
            AddStr(Attributes -> Paths, '{');
            AppendStr(Attributes -> Paths, Argument);
            AddStr(Attributes -> Paths, '}');
            AddStr(Attributes -> Paths, '\n');
        }
        if (StartsWith(Argument, "*")) 
        {
            String* s = FromStr(Argument);
            Usize Length = 0;
            for (int x = 0 ; s -> Chars[x] != '.' && s -> Chars[x] != '\0' ; x++) {
                Length++;
            }
            char* ArgumentValue = Substring(s -> Chars, 1, Length);
            Attributes -> TargetArchive = ArgumentValue;
        }
        else if (StartsWith(Argument, "-")) {
            String* s = FromStr(Argument);
            Usize Length = 0;
            for (int x = 0 ; s -> Chars[x] != '.' && s -> Chars[x] != '\0' ; x++) {
                Length++;
            }
            char* ArgumentValue = Substring(s -> Chars, 1, Length);
            ReadCallArgument(Attributes, ArgumentValue);
        }

        Count++;
    }
    printf("Compiling to \'%s\'\n", Attributes -> TargetArchive);
    return Attributes;
}

void ReadCallArgument(CompAttributes* Attributes, char* Argument) {
    if (Is(Argument, "o") || Is(Argument, "O")) {
        Attributes -> Target = MACHINE_CODE;
    }
    else if (Is(Argument, "c") || Is(Argument, "C")) {
        Attributes -> Target = C;
    }
}

void ReadPath(CompAttributes* CompilationAttributes, Alice_BigGirl* BigGirl) {
    int l = 0;
    int x = 0;
    char act = CompilationAttributes -> Paths -> Chars[x];
    while (CompilationAttributes -> Paths -> Chars[x] != '\0') 
    {
        if (act == '{') {
            x++;
            l = 0;
            while (CompilationAttributes -> Paths -> Chars[x + l] != '}') {
                l++;
            }
            char* Path = Substring(CompilationAttributes -> Paths -> Chars, x, x + l);
            Alice_CodeObject* NewCode = OpenMermaidFile(Path);
            GirlAppend(BigGirl, NewCode);
        }
        x++;
        act = CompilationAttributes -> Paths -> Chars[x];
    }
}