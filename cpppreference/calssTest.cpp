//
// Created by william on 2021/7/17.
//
#include <iostream>

namespace classTest
{
class Base
{
public:
    int a = 0;
};

class DerivedTest1 : public Base
{
};

// 没声明默认为private
class DerivedTest2 : Base
{
};

void classTest()
{
    DerivedTest1 derived1;
    DerivedTest2 derived2;
    std::cout << derived1.a << std::endl;
}
} // namespace classTest
