//
// Created by william on 2021/9/3.
//
#include "base.h"

void vextorTest()
{
    int array[100]{1};
    size_t arraySize = sizeof(array)/sizeof(array[0]);
    std::vector<int> vec(array, array + arraySize);
    int* arrayCopy = new int[arraySize];
    if (!vec.empty())
    {
        std::copy(&vec[0], &vec[0] + sizeof(int) * vec.size(), arrayCopy);
//        memcpy(arrayCopy, &vec[0], sizeof(int) * vec.size());
    }
    for (int i = 0; i < arraySize; ++i)
    {
        std::cout << arrayCopy[i] << std::endl;
    }
}