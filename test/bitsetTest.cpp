//
// Created by william on 2020/12/26.
//

#include <bitset>
#include <string>
#include "base.h"
// 二进制库

void bitsetTest()
{

    std::bitset<32> a; // a：32位二进制，全部都是0
    std::cout << a << std::endl;
    std::bitset<16> b(0xffff);
    std::cout << b << std::endl;
    std::bitset<32> c(0xffff);
    std::cout << c << std::endl;
    std::bitset<128> d(0xffff);
    std::cout << d << std::endl;
    std::bitset<32> e(156);
    std::cout << e << std::endl;
    std::string str{"111110000001"};
    // 从右边开始数
    std::bitset<32> f(str, 4, 5);
    std::cout << f << std::endl;
    bool isSet = a.any();
    if (isSet)
    {
        std::cout << "a里至少有一个1"<< std::endl;
    }

}