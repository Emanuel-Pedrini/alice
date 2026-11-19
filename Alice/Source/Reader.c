#include "External/Libraries.h"
#include "Headers/ArchiveManager.h"
#include "Headers/CStringUtils.h"
#include "Headers/InterpreterConstants.h"
#include "Headers/Lexer.h"
#include "Headers/Reader.h"
#include "Headers/Lexer.h"

CompAttributes* ReadCompilationAttributes(int NumberOfArguments, char** Arguments) 
{
    CompAttributes* Attributes = __AlAllocate(sizeof(CompAttributes));
    Attributes -> Paths = __AlStringFrom("");

    Usize Count = 0;
    Usize x = 0;

    printf("Including...\n");
    
    while (Count < NumberOfArguments) 
    {
        char* Argument = Arguments[Count];
        if (EndsWith(Argument, ALICE_SUFFIX)) 
        {
            __AliceString* s = __AlStringFrom(Argument);
            __AppendAlString(Attributes -> Paths, "{");
            __AppendAlString(Attributes -> Paths, Argument);
            __AppendAlString(Attributes -> Paths, "}");
            __AppendAlString(Attributes -> Paths, "\n");
        }
        if (StartsWith(Argument, "*")) 
        {
            __AliceString* s = __AlStringFrom(Argument);
            Usize Length = 0;
            for (int x = 0 ; s -> Chars[x] != '.' && s -> Chars[x] != '\0' ; x++) {
                Length++;
            }
            char* ArgumentValue = Substring(s -> Chars, 1, Length);
            Attributes -> TargetArchive = ArgumentValue;
        }
        else if (StartsWith(Argument, "-")) {
            __AliceString* s = __AlStringFrom(Argument);
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
    if (IsEqual(Argument, "c") || IsEqual(Argument, "C")) {
        Attributes -> Target = C;    
    }
    else {
        Attributes -> Target = LLVM;
    }
}

Bool ReadCompilationPaths(CompAttributes* CompilationAttributes, AliceManagementObject* BigGirl) {
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
            AliceCodeObject* NewCode = OpenAliceFile(Path);
            if (NewCode == NULL) 
            {
                return False;
            }
            GirlAppend(BigGirl, NewCode);
        }
        x++;
        act = CompilationAttributes -> Paths -> Chars[x];
    }
    return True;
}