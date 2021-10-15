//
// Created by william on 2021/1/3.
//

#include "base.h"

#include <sstream>

class Y
{};
class A
{
    int data; // 私有数据成员
    class B {}; // 私有嵌套类型
    enum  {a = 100}; // 私有枚举项
    friend class X; // 友元类的前置声明（描述类型说明符）
    friend Y; // 友元类的前置声明（简单类型说明符） C++11起
};

class X : A::B // OK 友元能访问 A::B
{
    A::B mx; // OK：友元的成员能访问 A::B
    class Y {
        A::B my; // OK：友元的嵌套成员能访问 A::B
    };
    int v[A::a]; // OK：友元的成员能访问 A::a
};

// 流插入与提取运算符往往声明为非成员友元
class MyClass
{
    int i;
    friend std::ostream& operator<<(std::ostream& out, const MyClass& o);
    friend std::istream& operator>>(std::istream& in, MyClass& o);

public:
    MyClass(int _i = 0) :
        i(_i) {}
};

std::ostream& operator<<(std::ostream& out, const MyClass& mc)
{
    return out << mc.i;
}

std::istream& operator>>(std::istream& in, MyClass& mc)
{
    return in >> mc.i;
}

void friendTest()
{
    MyClass mc{ 7 };
    std::cout << mc << '\n';
    std::istringstream("100") >> mc;
    std::cout << mc << "\n";
}
