#include "Headers/AliceTypes.h"
__AliceString* __NewAlString()
{
    __AliceString* __AlString = malloc(sizeof(__AliceString));
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

}

__AliceVoid __AppendAlString(__AliceString* __AlString, const char* __Source)
{
    const int __SourceLen = strlen(__Source);
    const int __PretendedSize = (__AlString -> SSize + __SourceLen) + 1;
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
}