#include "Utilities/AliceDevArraySortsTools.h"

Dsize* __InsertionSort(Dsize* __Array, Usize __ArraySize)
{
    for 
    (
        Usize __Interator = 1;
        __Interator < __ArraySize;
        __Interator++
    )
    {
        Dsize __Key = __Array[__Interator];
        Usize __HelperInterator = __Interator;
        while (__HelperInterator > 0 && __Array[__HelperInterator - 1] > __Key)
        {
            __Array[__HelperInterator] = __Array[__HelperInterator - 1];
            __HelperInterator--;
        }
        __Array[__HelperInterator] = __Key;
    }
    Dsize* __ArrayPointerCopy = __Array;
    return __ArrayPointerCopy;
}

Dsize* __BubbleSort(Dsize* __Array, Usize __ArraySize)
{

}

__AliceString* ConversionToString__DsizeArray(Dsize* __Array, Usize __ArraySize)
{
    __AliceString* __Result = __AlStringFrom("[ ");
    for (Usize __Interator = 0;
        __Interator < __ArraySize;
        __Interator++)
    {
        char __Buffer[32];
        snprintf(__Buffer, sizeof(__Buffer), "%d", __Array[__Interator]);
        __AppendAlString(__Result, __Buffer);
        if (__Interator + 1 < __ArraySize) __AppendAlString(__Result, " , ");
    }
    __AppendAlString(__Result, " ]");
    return __Result;
}

