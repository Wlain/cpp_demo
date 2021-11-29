//
// Created by william on 2020/7/1.
//

#include "assignment.h"
#include "base.h"
#include "mat4.h"

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
extern void constexprTest();
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
extern void anyTest();
extern void staticTest();
extern void crashTest();
extern void restrictTest();

extern void vulkanTest();
extern void glTest();
extern int metalTest(int argc, const char* argv[]);

namespace graphics
{
extern void assignment1();
extern void assignment2();
extern void assignment3(ShardingType type);
extern void assignment4();
extern void assignment5();
extern void pathTracingTriangle();
} // namespace graphics

int main(int argc, const char* argv[])
{
    restrictTest();
    Mat4f d(0.707107, 0, -0.707107, 0, -0.331295, 0.883452, -0.331295, 0, 0.624695, 0.468521, 0.624695, 0, 4.000574, 3.00043, 4.000574, 1);
    std::cerr << d << std::endl;
    d.invert();
    std::cerr << d << std::endl;
    return 0;
}
