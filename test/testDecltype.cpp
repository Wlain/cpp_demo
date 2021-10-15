//
// Created by william on 2020/7/1.
//

#include "base.h"

using std::cin;
using std::cout;
using std::endl;

int func1()
{
    return 10;
}
auto func2()
{
    return 'g';
} // C++14

void testDecltype()
{
    decltype(func1()) x;
    decltype(func2()) y = func2();
    cout << "testDecltype:" << typeid(x).name() << endl;
    cout << "testDecltype:" << typeid(y).name() << endl;
}
