//
// Created by william on 2020/7/1.
//

#include "base.h"

[[maybe_unused]] int arr[3]; // 全局数组:全局/静态区

void testMemoryAlloc()
{
    int a;                     // 函数里面的局部变量：栈区
    char* p;                   // 函数里面的局部变量： 栈区
    const char* str = "hello world"; // 函数里面的局部变量：栈区，"hello
                               // world":是一个字符串常量，位于常量区
}

// pi：函数的形式参数，也就是局部变量:栈区
void testMemoryAlloc1(int* pi)
{
    char* pc;                             // 局部变量：栈区
    pc = static_cast<char*>(new char[8]); // char[8]：堆区
}
