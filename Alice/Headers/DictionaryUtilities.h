#ifndef _DictionaryUtilitiesH_
#define _DictionaryUtilitiesH_

#include "ArrayUtilities.h"
typedef struct StructDictionary 
{
    Vector* Keys;
    Vector* Values;
} Dictionary;

typedef struct StructTable 
{
    Vector* Values;
} Table;

Table* CreateTable();
Bool Has(Table* Tab, const char* Value);
void Value(Table* Tab, const char* Value);

Dictionary* CreateDict(Usize ValueSize);
void  Key(Dictionary* Dict, const char* Key, const void* Value);
Bool  Contains(const char* Key);
Vector* Keys(Dictionary* Dict);
Vector* Values(Dictionary* Dict);
void* GetKeyV(Dictionary* Dict, const char* Key);
void* GetValueK(Dictionary* Dict, const void* Value);

#define DICT(D, V) Dictionary* D = CreateDict(sizeof(V));
#define TABLE(T) Table* T = CreateTable();
#endif