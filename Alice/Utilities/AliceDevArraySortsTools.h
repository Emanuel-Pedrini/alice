#ifndef _ArraySortsH_
#define _AraySortsH_
#include "Headers/AliceDevelopmentTypes.h"
Dsize* __InsertionSort(Dsize* __Array, Usize __ArraySize);
Dsize* __BubbleSort(Dsize* __Array, Usize __ArraySize);
Dsize* __DinahSort(Dsize* __Array, Usize __ArraySize);

__AliceString* ConversionToString__DsizeArray(Dsize* __Array, Usize __ArraySize);
#endif