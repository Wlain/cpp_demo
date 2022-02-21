//
// Created by william on 2020/7/1.
//

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
extern float getUsageMemory();
extern void anyTest();
extern void staticTest();
extern void staticTest2();
extern void crashTest();
extern void restrictTest();
/// 验证一个异步崩溃的demo
extern void asyncTestDemo();

namespace geekTimeTest
{
extern void stackUnwindTest();
extern void shapeWrapTest();
extern void uniquedPtrTest();
extern void sharedPtrTest();
extern void lifeTimeTest();
extern void nrvoTest();
extern void forwardTest();
extern void outputContainerTest();
extern void vectorTest();
extern void listTest();
extern void queueTest();
extern void stackTest();
extern void hashTest();
extern void priorityQueueTest();
extern void unorderedTest(); /// 性能很慢，数据小于一千，慎用！！！
extern void arrayTest();
extern void binaryLiteralsTest();
extern void literalsTest();
extern void userLiteralsTest();
extern void utfTest();
} // namespace geekTimeTest

extern void vulkanTest();
extern void glTest();
extern int metalTest(int argc, const char* argv[]);

namespace cg
{
extern void phongTest();
extern void bilinearInterpolationTest();
extern void trilinearInterpolationTest();
} // namespace cg

namespace graphics
{
extern void assignment1();
extern void assignment2();
extern void assignment3(ShardingType type);
extern void assignment4();
extern void assignment5();
extern void pathTracingTriangle();
} // namespace graphics

namespace rayTracing
{
extern void chapter1();
extern void chapter3();
extern void chapter4();
extern void chapter5();
extern void chapter5_1();
extern void chapter6();
extern void chapter7();
extern void chapter8();
extern void chapter9();
extern void chapter10();
extern void chapter11();
extern void chapter12();
} // namespace rayTracing

extern void imageProcessTest();
extern void colorTransferBetweenImages();
extern int qtTest(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    qtTest(argc, argv);
    return 0;
}
