//
// Created by cwb on 2022/3/3.
//
#include <iostream>

class B
{
public:
    B() = default;
    virtual ~B() = default;
    virtual void test(int a = 1)
    {
        std::cout << a << std::endl;
    }
};

class D : public B
{
public:
    D() = default;
    virtual ~D() = default;
    void test(int a = 10) override
    {
        std::cout << a - 1 << std::endl;
    }
};

void virtualFuncTest()
{
    B* d = new D();
    d->test();
    std::cout << "输出的是 0，可能用户以为的默认参数是10，所以以为输出是9，需要注意" << std::endl;
}
