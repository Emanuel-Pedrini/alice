#ifndef _IntermediateRepresentationH_
#define _IntermediateRepresentationH_

#include "Headers/AliceTypes.h"

typedef struct Struct__AliceIRValue AliceIRValue;
typedef struct Struct__AliceIRInstruction AliceIRInstruction;
typedef struct Struct__AliceIRBlock AliceIRBlock;
typedef struct Struct__AliceIRFunction AliceIRFunction;
typedef struct Struct__AliceIRModule AliceModule;
typedef struct Struct__AliceIRBuilder AliceIRBuilder;
typedef struct Struct__AliceIRContext AliceIRContext;

typedef enum Enum__AliceIRValueKind
{
    IRValue_None,
    IRValue_Constant,
    IRValue_Temporary,
    IRValue_Slot
} ALiceIRValueKind;

struct AliceIRValue
{
    ALiceIRValueKind Kind;
    __AliceType* Type;
};

struct AliceIRInstruction
{

};

struct AliceIRBlock
{

};

struct AliceIRFunction
{
    __AliceVector* Blocks; // Vector<AliceIRBlock*>*
};

struct AliceModule
{
    __AliceVector* Functions; // Vector<AliceIRFunction*>*
};

struct AliceIRBuilder
{

};

struct AliceIRContext
{
    
};

#endif