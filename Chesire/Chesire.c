#include "Headers/Manager.h"
#include "Headers/Utils.h"
#include "Control/Commands.h"
#include "Headers/State.h"
#include "Control/Version.h"

int main(int ArgumentCount, char* Arguments[]) {
    ChesireState LocalState;
    LocalState.Actual = "";
    LocalState.Pointer = 1;
    LocalState.Arguments = Arguments;
    ReadArguments(&LocalState);
    return 0;
}