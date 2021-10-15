//
// Created by william on 2020/7/1.
//
#include "base.h"

using std::cin;
using std::cout;
using std::endl;

void testConstantPointer()
{
    // 常量声明
    const int x = 0;
    // 常量不可变

    // 常量指针所指数据不可变
    int y = 2;
    const int* p = &x;
    const int* q = &y;
    // 指针常量不可变
    int* r = &y;
    int z = 5;
    *r = 20;
    cout << "testConstantPointer"
         << "*r:" << *r << ",y:" << y << endl;
    // 常指针常量
    const int* const s = &x;
    const int* const t = &y;
    cout << "testConstantPointer"
         << "*s:" << *s << endl;
    cout << "testConstantPointer"
         << "*t:" << *t << endl;
    const char* str = "hello";
    auto p1 = &y;
    auto p2 = "world";
    auto const p3 = "!";
    auto& p4 = "!";
    cout << "";
}
