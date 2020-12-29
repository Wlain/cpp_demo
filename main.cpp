//
// Created by william on 2020/7/1.
//
#include "include/base.h"
#include "include/basic_timer.h"
#include "vec2f.h"
#include "vector2.h"
#include <array>
#include <random>
#include <regex>

extern void testAuto();
extern void testDecltype();
extern void testConstantPointer();
extern void callbackTest();
extern void ifTest1();
extern void ifTest2();
extern void ifSwitch1();
extern void ifSwitch2();
extern void lambdaTest();
extern void testModern01();
extern void emplace_backTest();
extern void referenceTest();
extern void bitsetTest();
extern void staticAssertTest();
extern void extendedFriendDeclarationsTest();
extern void autoTest();

int Vec2f::constructorCount = 0;
int Vec2f::destructorCount = 0;
// 必须在定义类的文件中对静态成员变量进行一次说明,或初始化。否则编译能通过，链接不能通过。
using namespace std;

int main()
{
    autoTest();
    return 0;
}
