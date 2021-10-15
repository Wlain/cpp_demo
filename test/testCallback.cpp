//
// Created by william on 2020/11/13.
//

// 回调函数：
// 把一个函数callback的指针（地址）pf作为参数传递给另一个caller，并通过函数指针pf调用callback函数，称callback函数为回调函数，简而言之，回调函数啊就是一个通过函数指针调用的函数

#include "base.h"

// 函数指针
template <typename T>
int add(T a, T b)
{
    return a + b;
}

void callbackTest()
{
    int (*p)(int, int);
    p = add;
    int a = (*p)(1, 2);
    int b = p(2, 2);
    std::cout << "a:" << a << std::endl;
    std::cout << "b:" << b << std::endl;
}