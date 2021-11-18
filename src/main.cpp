//
// Created by william on 2020/7/1.
//

#include <array>
#include <random>
#include "assignment.h"
#include "base.h"

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

namespace threadTest
{
extern void threadTest();
}

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
extern void eigen3Test();
extern void opencvTest();
extern void optionalTest();
extern void transformTest();
extern void timerTest();
extern float getUseageMemory();
extern void vulkanTest();

namespace graphics
{
extern void assignment1();
extern void assignment2();
extern void assignment3(ShardingType type);
extern void assignment4();
} // namespace graphics

int main()
{
    vulkanTest();
    return 0;
}