//
// Created by william on 2022/5/12.
//
#include "outputContainer.h"
#include <algorithm>
#include <array>
#include <functional>

void replaceTest()
{
    std::array<int, 10> array{ 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    std::cout << array << std::endl;
    std::replace(array.begin(), array.end(), 8, 88);
    std::cout << array << std::endl;
    std::replace_if(array.begin(), array.end(),
                    std::bind(std::less<int>(), std::placeholders::_1, 5), 55);
    std::cout << array << std::endl;
}