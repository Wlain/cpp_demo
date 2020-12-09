//
// Created by william on 2020/7/1.
//

#include "base.h"

using std::cin;
using std::cout;
using std::endl;

const int base = 0;
int c = 1;

auto max(int x, int y)
{
    return x > y ? x : y;
}

void testAuto()
{
    // auto 变量必须在定义时初始化
    auto x = 3;
    auto y{ 42 };
    // 定义在auto序列的变量必须始终推导成统一类型
    auto x1{ 1 }, x2{ 2 };
    // 如果初始化表达式是引用或者const，则去除引用或者const语义
    int y1{ 42 }, &y2{ y1 };
    auto y3{ y2 };
    cout << "testAuto:" << typeid(y3).name() << endl;
    // 如果auto关键字带上&号，则不去除引用或者const语义
    auto& z1{ y2 };
    cout << "testAuto:" << typeid(z1).name() << endl;
    // 初始化表达式为数组时，auto关键字推导类型为指针
    int p[3]{ 1, 2, 3 };
    auto p1{ p };
    cout << "testAuto:" << typeid(p1).name() << endl;
    // 若表达式为数组且auto带上&, 则推导类型为数组
    auto& p2{ p };
    cout << "testAuto:" << typeid(p2).name() << endl;
    // C++14中，auto可以作为函数的返回值类型和参数类型
    cout << "testAuto:" << max(x1, x2) << endl;
}
