//
// Created by william on 2020/12/9.
//
// 带有初始化器的if语句
#include "base.h"
#include <random>

static std::random_device r;
// 选择 1 与 6 间的随机数
static std::default_random_engine e1(r());
static std::uniform_int_distribution<int> uniformDist(0, 100);

auto randomTest()
{
    return uniformDist(e1);
}


int foo(int arg)
{
    return (arg);
}

void ifTest1()
{
    auto x = foo(42);
    if (x > 40)
    {
        std::cout << "ifTest1:x > 40" << std::endl;
    }
    else
    {
        std::cout << "ifTest1:x <= 40" << std::endl;
    }
}

void ifTest2()
{
    if (auto x = foo(42); x > 40)
    {
        std::cout << "ifTest2:x > 40" << std::endl;
    }
    else
    {
        std::cout << "ifTest2:x <= 40" << std::endl;
    }
}

void ifSwitch1()
{
    int x = randomTest();
    switch (x)
    {
    case 1:
        std::cout << "1" << std::endl;
        break;
    default:
        std::cout << "default" << std::endl;
        break;;
    }
}

void ifSwitch2()
{
    switch (int x = randomTest(); x)
    {
    case 1:
        std::cout << "1" << std::endl;
        break;
    default:
        std::cout << "default" << std::endl;
        break;;
    }
}
