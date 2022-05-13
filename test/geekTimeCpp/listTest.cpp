//
// Created by william on 2021/12/12.
//
#include "outputContainer.h"

#include <algorithm> // std::sort
#include <iostream>  // std::cout/endl
#include <list>      // std::list
#include <vector>    // std::vector

namespace geekTimeTest
{
void listTest()
{
    std::list<int> lst{1, 7, 2, 8, 3};
    std::vector<int> vec{1, 7, 2, 8, 3};
    sort(vec.begin(), vec.end());
    lst.sort();
    std::cout << lst << std::endl;
    std::cout << vec << std::endl;
}
} // namespace geekTimeTest