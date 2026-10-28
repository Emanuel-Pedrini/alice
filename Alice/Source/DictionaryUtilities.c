#include "Headers/DictionaryUtilities.h"
#include "Headers/ArrayUtilities.h"
#include "Headers/StringUtilities.h"
#include "External/Libraries.h"

Dictionary* CreateDict(Usize ValueSize)
{
    Dictionary* Dict = malloc(sizeof(Dictionary));

    Vector* Keys = malloc(sizeof(Vector));
    New(Keys, sizeof(String));

    Vector* Values = malloc(sizeof(Vector));
    New(Values, ValueSize);

    Dict -> Keys = Keys;
    Dict -> Values = Values;
    return Dict;
}

void  Key(Dictionary* Dict, const char* Key, const void* Value) 
{
    Add(Dict -> Keys, FromStr(Key));
    Add(Dict -> Values, Value);
}

Bool  Contains(const char* Key) 
{
    return False;
}

Vector* Keys(Dictionary* Dict)
{
    Vector* P = Dict -> Keys;
    return P;
}

Vector* Values(Dictionary* Dict)
{
    Vector* P = Dict -> Values;
    return P;
}

void* GetKeyV(Dictionary* Dict, const char* Key)
{
    String* A = FromStr(Key);
    for (int i = 0 ; i < Dict -> Keys -> size; i++)
    {
        String* B = Get(Dict -> Keys, i);
        void* C = Get(Dict -> Values, i);
        if (CompStr(A, B))
        {
            return C;
        }
    }
    return NULL;
}

void* GetValueK(Dictionary* Dict, const void* Value)
{
}

Table* CreateTable()
{
    Table* Tab = malloc(sizeof(Table));
    Vector* Values = malloc(sizeof(Vector));
    New(Values, sizeof(String));
    Tab -> Values = Values;
    return Tab;
}

Bool Has(Table* Tab, const char* Value)
{
    String* A = FromStr(Value);
    for (int i = 0 ; i < Tab -> Values -> size ; i++)
    {
        String* B = Get(Tab -> Values, i);
        if (CompStr(A, B))
        {
            return True;
        }
    }
    return False;
}

void Value(Table* Tab, const char* Value)
{
    Add(Tab -> Values, FromStr(Value));
}