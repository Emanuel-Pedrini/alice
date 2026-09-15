#include <stdlib.h>
#include <stdio.h>

#include "Headers/State.h"
#include "Headers/Manager.h"
#include "Headers/Utils.h"
#include "Control/Commands.h"

char* GetArgument(ChesireState* ChesireState, int Offset) 
{
    char* ActualArgument = ChesireState -> Arguments[ChesireState -> Pointer + Offset];
    return ActualArgument;
}

void NextArgument(ChesireState* ChesireState, int Offset)
{
    ChesireState -> Pointer += Offset;
    char* ActualArgument = ChesireState -> Arguments[ChesireState -> Pointer];
}

void ReadArguments(ChesireState* ChesireState) 
{
    while (GetArgument(ChesireState, 0) != NULL) {
        if (IsEqual(GetArgument(ChesireState, 0), CREATE_COMMAND)) 
        {
            CreateChesire(GetArgument(ChesireState, 1));
        }
        if (IsEqual(GetArgument(ChesireState, 0), INITIALIZE_COMMAND)) 
        {
            CreateChesire("");
        }
        printf("%s\n", GetArgument(ChesireState, 0));
        NextArgument(ChesireState, 1);
    }
}