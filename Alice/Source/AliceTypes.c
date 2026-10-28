#include "Headers/AliceTypes.h"
#include "External/Libraries.h"

#define __AlString_InitialCapacity 16

__AliceString* __NewAlString()
{
    __AliceString* __AlString = malloc(sizeof(__AliceString));
    if(__AlString == NULL) return NULL;
    __AlString -> SSize = 0;
    __AlString -> SCapacity = __AlString_InitialCapacity;
    __AlString -> Chars = malloc(__AlString -> SCapacity);
    __AlString -> Chars[0] = '\0';
    return __AlString;
}

__AliceString* __AlStringFrom(const char* __Source)
{
    __AliceString* __AlString = __NewAlString();
    __AppendAlString(__AlString, __Source);
    return __AlString;
}

__AliceString* __ConcatenateAlString(Usize __Qtd, ...)
{
    va_list __Arguments;
    va_start(__Arguments, __Qtd);
    __AliceString* __AlString = __NewAlString();
    for (int __Interator = 0; __Interator < __Qtd; __Interator++)
    {
        __AliceString* __StringElement = va_arg(__Arguments, __AliceString*);
        __AppendAlString(__AlString, __StringElement -> Chars);
    }
    va_end(__Arguments);
    return __AlString;
}
__AliceString* __JoinAlString(const char* __Separator, Usize __Qtd, ...)
{
    va_list __Arguments;
    va_start(__Arguments, __Qtd);
    __AliceString* __AlString = __NewAlString();
    for (int __Interator = 0; __Interator < __Qtd; __Interator++)
    {
        if (__Interator == __Qtd - 1)
        {
            __AliceString* __StringElement = va_arg(__Arguments, __AliceString*);
            __AppendAlString(__AlString, __StringElement -> Chars);
        }
        else
        {
            __AliceString* __StringElement = va_arg(__Arguments, __AliceString*);
            __AppendAlString(__AlString, __StringElement -> Chars);
            __AppendAlString(__AlString, __Separator);
        }
    }
    va_end(__Arguments);
    return __AlString;
}

__AliceVoid __AppendAlString(__AliceString* __AlString, const char* __Source)
{
    Usize __SourceLen = strlen(__Source);
    Usize __PretendedSize = (__AlString -> SSize + __SourceLen) + 1;
    if (__PretendedSize >= __AlString -> SCapacity)
    {
        Usize __NewCapacity = ((__PretendedSize * 2) + 1);
        char* __NewElements = realloc(__AlString -> Chars, __NewCapacity * sizeof(char));
        if (__NewElements == NULL)
        {
            free(__NewElements);
            return;
        }
        __AlString -> Chars = __NewElements;
        __AlString -> SCapacity = __NewCapacity;
    }
    for (int __Interator = 0; __Source[__Interator] != '\0'; __Interator++)
    {
        char __CharInterator = __Source[__Interator];
        __AlString -> Chars[__AlString -> SSize] = __CharInterator;
        __AlString -> SSize = __AlString -> SSize + 1;
    }
    __AlString -> Chars[__AlString -> SSize] = '\0';
}