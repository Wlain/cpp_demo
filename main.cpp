//
// Created by william on 2020/7/1.
//
#include "include/base.h"
#include "vec2f.h"
#include "include/basic_timer.h"
#include <vector>


extern void testAuto();
extern void testDecltype();
extern void testConstantPointer();
extern void callbackTest();

int Vec2f::constructorCount = 0;
int Vec2f::destructorCount = 0;
// 必须在定义类的文件中对静态成员变量进行一次说明,或初始化。否则编译能通过，链接不能通过。

int main()
{
    //  testAuto();
    //  testDecltype();
    //  testConstantPointer();
    callbackTest();
    return 0;
}

