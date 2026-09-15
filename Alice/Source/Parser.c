#include "Headers/Parser.h"
#include "External/Libraries.h"
void Parsenize(Alice_BigGirl* BigGirl, Vector* Tokens) {
    printf("\n");
    for (int x = 0;x < Len(Tokens);x++) {
        Alice_Token* y = Get(Tokens, x);
        printf("(%d) \"%s\"\n", y -> Type, Substring(BigGirl -> SourceCode -> Chars, y -> Start, y -> Start + y -> Length));
    }
}