//
// Created by william on 2022/4/30.
//
#include <iostream>

void charTest()
{
    char c1 = 'C';
    char c2 = 80;
    char c3 = 0x50;
    char16_t c4 = u'于';
    char32_t c5 = U'于';
    std::cout << c1 << ":" << c2 << ":" << c3 << std::endl;
    std::cout << +c1 << ":" << +c2 << ":" << +c3 << std::endl;
    unsigned char a = 255;
    unsigned char b = 1;
    unsigned char c = a + b;
    auto d = a + b;
    int e = a + b;
    std::cout << e << std::endl;
}