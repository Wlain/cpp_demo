//
// Created by william on 2021/12/13.
//
#include "outputContainer.h"

#include <array>
#include <iostream>
#include <map>

typedef std::array<char, 8> myKey;

namespace geekTimeTest
{
void arrayTest()
{
    std::map<myKey, int> mp;
    myKey mykey{"hello"};
    mp[mykey] = 5;  // OK
    std::cout << mp << std::endl;
}
} // namespace geekTimeTest