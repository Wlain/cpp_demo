//
// Created by william on 2020/7/1.
//
#include "include/base.h"
#include <array>
#include <random>
#include "opencvTest.h"

namespace enableSharedFromThisTest
{
extern void enableSharedFromThisTest();
}

namespace mutexTest
{
extern void lockTest();
extern void tryLockTest();
extern void mutexTest();
} // namespace mutexTest

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
extern void friendTest();
extern void constexpTest();
extern int eraserOverlapIntervals(std::vector<std::vector<int>>& intervals);
extern void sharePtrTest();
extern void moveTest();
extern void cpp03Test();
extern void cpp11Test();
extern void cpp17Test();
extern void cpp20Test();
extern void stringViewTest();
extern void dtorTest();
extern void lambdaErrorTest();
extern void interviews();
extern void regexTest();
extern void bindTest();
extern void removeReferenceTest();
extern void lockGuardTest();
extern void atomicTest();
extern void underlying_type_t_test();
extern void fileIOTest();
extern void raiseTest();
extern void unorderedMapTest();
extern void stringTest();
extern void addAddTest();
extern void luaTest();
extern std::string toHex(int num);

int main()
{
    OpencvTest test;
    test.show();
    std::cout << "hello world" << std::endl;
    return 0;
}