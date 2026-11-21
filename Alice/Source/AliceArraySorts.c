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
        // printf("(%d) -> %s\n", __Interator, ConversionToString__DsizeArray(__Array, __ArraySize) -> Chars);
        __Array[__HelperInterator] = __Key;
    }
    Dsize* __ArrayPointerCopy = __Array;
    return __ArrayPointerCopy;
}

Dsize* __BubbleSort(Dsize* __Array, Usize __ArraySize)
{
    Bool __Swapped = True;
    Usize __Interator = 0;
    for (__Interator = 0;;__Interator++)
    {
        __Swapped = False;
        Usize __NextIndex = __Interator + 1;
        printf("\nD> %d\n", __NextIndex);
        Dsize __NextValue = __Array[__NextIndex];
        printf("\nN> %d\n", __NextValue);
        printf("\nI> %d\n", __Array[__Interator]);
        if (__Array[__Interator] > __NextValue)
        {
            printf("\n>> %d\n", __Array[__Interator]);
            __Array[__NextIndex] = __Array[__Interator];
            __Array[__Interator] = __NextValue;
            __Swapped = True;
        }
        if ((__Interator < __ArraySize)) {

        }
    }
    Dsize* __ArrayPointerCopy = __Array;
    return __ArrayPointerCopy;
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

