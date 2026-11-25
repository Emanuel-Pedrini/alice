#ifndef _ExternalLibrarisH_
#define _ExternalLibrarisH_

#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
    #define __AliceMakeDir(Path) _mkdir(Path);
#else
    #include <sys/types.h>
    #define __AliceMakeDir(Path) mkdir(Path, 0755);
#endif

/* External Libraries */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <stddef.h>

// LLVM 
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Error.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Transforms/PassBuilder.h>

#endif